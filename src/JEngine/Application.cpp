#include "GLManager.h"
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "Application.h"
#include "StateManager.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "Random.h"
#include "ImguiManager.h"
#include "imgui.h"
#include "Debug.h"
#include "InputHandler.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
int				APP::m_samples = 0;
int				APP::m_buffers = 0;
SDL_Event		APP::m_pEvent;
SDL_Window*		APP::m_pWindow = nullptr;
SDL_Surface		*APP::m_pSurface = nullptr, *APP::m_pIcon= nullptr;
SDL_GLContext	APP::m_pContext = nullptr;
APP::InitData	APP::m_Data = { "demo", "../resource/ico/main.ico", false, 800, 600 };
bool			APP::m_IMGUI = false;

void Application::Run(bool _imgui)
{
	m_IMGUI = _imgui;

	// Pop console window 
	DEBUG_LEAK_CHECKS(-1);

	// and check memory leak
	DEBUG_CREATE_CONSOLE();

	// Open application
	if (Initialize())
		Update();

	Close();
	
	// Delete console window
	DEBUG_DESTROY_CONSOLE();
}

bool Application::Initialize()
{
	/*************** Init Data **************/

	// Assign app init data
	JSON::ReadFile(ASSET::m_initDirectory.c_str());

	CR_RJValue title = JSON::GetDocument()["Title"];
	CR_RJValue fullscreen = JSON::GetDocument()["Fullscreen"];
	CR_RJValue width = JSON::GetDocument()["Width"];
	CR_RJValue height = JSON::GetDocument()["Height"];
	CR_RJValue icon = JSON::GetDocument()["Icon"];

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()
		&& icon.IsString()) {

		m_Data.m_title.assign(title.GetString());
		m_Data.m_icon.assign(icon.GetString());
		m_Data.m_isFullScreen = fullscreen.GetBool();
		m_Data.m_width = width.GetInt();
		m_Data.m_height = height.GetInt();
	}

	else {
		jeDebugPrint("!Application - Wrong init data.\n");
		return false;
	}

	// Plant random seed
	Random::PlantSeed();

	/*************** SDL **************/
	bool initedSDL = InitSDL();

	/*************** OpenGL **************/
	GLM::Resize(m_Data.m_width, m_Data.m_height);
	bool initedGL = GLM::Init();

	/**************** IMGUI **************/
	bool initedIMGUI = IMGUI::Init(m_pWindow);
	IMGUI::AddEditorFunc(APP::EditorUpdate);
	IMGUI::AddEditorFunc(GLM::EditorUpdate);
	IMGUI::AddEditorFunc(STATE::EditorUpdate);

	/**************** Assets **************/
	// Load info from json files
	ASSET::LoadAssets();		

	// Generate built-in components
	bool initedBuiltInComponents = ASSET::SetBuiltInComponents();
	
	// Load state info
	// Bind systems here
	bool initedStates = STATE::Init(m_pWindow);	

	return initedSDL && initedGL && initedIMGUI 
		&& initedBuiltInComponents && initedStates;
}

void Application::Update()
{
	// Update the surface
	while (STATE::GetStatus()
		!= STATE::StateStatus::STATE_QUIT) {

		// Update state manager
		STATE::Update(&m_pEvent);

		// Update sdl window
		SDL_UpdateWindowSurface(m_pWindow);

	}	// while (STATE::GetStatus()
		// != STATE::StateStatus::STATE_QUIT) {
}

void Application::Close()
{
	STATE::Close();			// Remove systems and states
	ASSET::UnloadAssets();	// Clear loaded assets
	JSON::Close();			// Clear document
	GLM::Close();			// Close SDL GL
	IMGUI::Close();			// Close imgui manager

	CloseSDL();				// Close sdl window
}

bool Application::InitSDL()
{
	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		// Print error message
		jeDebugPrint("!Application - SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Initialize png loading
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
	{
		jeDebugPrint("!Application - SDL_image could not initialize. SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	// Call opengl 4.5
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	//SDL_SetVideoMode();
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//Create window
	m_pWindow = SDL_CreateWindow(m_Data.m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_Data.m_width, m_Data.m_height, SDL_WINDOW_OPENGL);

	if (!m_pWindow) {
		jeDebugPrint("!Application - Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Set window icon
	m_pIcon = IMG_Load(m_Data.m_icon.c_str());

	SDL_SetWindowIcon(m_pWindow, m_pIcon);

	// Get context
	m_pContext = SDL_GL_CreateContext(m_pWindow);

	SDL_SetWindowFullscreen(m_pWindow, m_Data.m_isFullScreen);

	// Get window surface
	m_pSurface = SDL_GetWindowSurface(m_pWindow);

	// Fill the surface white
	SDL_FillRect(m_pSurface, nullptr, SDL_MapRGB(m_pSurface->format, 0xFF, 0xFF, 0xFF));

	return true;
}

void Application::CloseSDL()
{
	// Delete m_pIcon
	SDL_FreeSurface(m_pIcon);

	// Destroy
	SDL_GL_DeleteContext(m_pContext);

	//Destroy window
	SDL_DestroyWindow(m_pWindow);

	//Quit SDL subsystems
	SDL_Quit();
}

void Application::EditorUpdate(const float /*_dt*/)
{
	// Basic debug window
	ImGui::Begin("Debug");
	ImGui::Text("*JEngine Frame Time: %.11f", STATE::m_frameTime);
	ImGui::Text("*Resolution: %d x %d", m_Data.m_width, m_Data.m_height);

	if (!m_Data.m_isFullScreen) {
		ImGui::Text("*Screen Mode: Window Mode");
		if (ImGui::Button("Fullscreen")) {
			m_Data.m_isFullScreen = true;
			SDL_SetWindowFullscreen(m_pWindow, m_Data.m_isFullScreen);
		}
	}

	else {
		ImGui::Text("*Screen Mode: Fullscreen");
		if (ImGui::Button("Window Mode")) {
			m_Data.m_isFullScreen = false;
			SDL_SetWindowFullscreen(m_pWindow, m_Data.m_isFullScreen);
		}
	}

	ImGui::End();
}

jeEnd
