#include "GLManager.h"
#include "SDL_opengl.h"
#include "Application.h"
#include "StateManager.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "Random.h"
#include "ImguiManager.h"
#include "imgui.h"
#include "Debug.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
int				APP::m_samples = 0;
int				APP::m_buffers = 0;
SDL_Event		APP::m_pEvent;
SDL_Window*		APP::m_pWindow = nullptr;
SDL_Surface*	APP::m_pSurface = nullptr;
SDL_GLContext	APP::m_pContext = nullptr;
APP::InitData	APP::m_Data = { "demo", false, 800, 600 };
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

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()) {

		m_Data.m_title.assign(title.GetString());
		m_Data.m_isFullScreen = fullscreen.GetBool();
		m_Data.m_width = width.GetInt();
		m_Data.m_height = height.GetInt();
	}

	else {
		JE_DEBUG_PRINT("!Application - Wrong init data.\n");
		return false;
	}

	/*************** SDL **************/

	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		// Print error message
		JE_DEBUG_PRINT("!Application - SDL could not initialize. SDL_Error: %s\n", SDL_GetError());
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
		JE_DEBUG_PRINT("!Application - Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Get context
	m_pContext = SDL_GL_CreateContext(m_pWindow);

	SDL_SetWindowFullscreen(m_pWindow, m_Data.m_isFullScreen);

	// Get window surface
	m_pSurface = SDL_GetWindowSurface(m_pWindow);

	// Fill the surface white
	SDL_FillRect(m_pSurface, nullptr, SDL_MapRGB(m_pSurface->format, 0xFF, 0xFF, 0xFF));

	/*************** Open GL **************/
	GLM::Resize(m_Data.m_width, m_Data.m_height);
	GLM::initSDL_GL();
	
	/**************** IMGUI **************/
	IMGUI::Init(m_pWindow);
	IMGUI::AddEditorFunc(APP::EditorUpdate);
	IMGUI::AddEditorFunc(GLM::EditorUpdate);
	IMGUI::AddEditorFunc(STATE::EditorUpdate);
	
	/**************** Built-in **************/
	Random::PlantSeed();	// Plant random seed
	ASSET::Load();			// Load info from json files
	STATE::Init(m_pWindow);	// Bind systems here

	return true;
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
		// != STATE::StateStatus::S_QUIT) {
}

void Application::Close()
{
	STATE::Close();		// Remove systems and states
	ASSET::Unload();	// Clear loaded assets
	GLM::CloseSDL_GL(); // Close SDL GL
	IMGUI::Close(); 	// Close imgui manager
	
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

JE_END
