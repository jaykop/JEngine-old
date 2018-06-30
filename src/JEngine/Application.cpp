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

#define jeCheck(c) if (!c) return false

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Event		APP::pEvent_;
SDL_Window*		APP::pWindow_ = nullptr;
SDL_Surface		*APP::pSurface_ = nullptr, *APP::pIcon_= nullptr;
SDL_GLContext	APP::pContext_ = nullptr;
APP::AppData	APP::data_ = { "demo", "../resource/ico/main.ico", false, 800, 600 };
bool			APP::activateIMGUI_ = false, APP::openCMD_ = false;

void Application::Run(bool imgui)
{
	activateIMGUI_ = imgui;
	if (activateIMGUI_)
		jeDebugPrint("*Application - IMGUI Activated.\n");

	// Initialize app info
	// and update
	if (Initialize())
		Update();

	// Wrap up application
	Close();
	
}

void Application::CreateConsole()
{
	// Pop console window 
	// and check memory leak
	if (!openCMD_) {
		DEBUG_LEAK_CHECKS(-1);
		DEBUG_CREATE_CONSOLE();

		openCMD_ = true;
	}
}

void Application::CloseConsole()
{
	if (openCMD_) {
		// Delete console window
		DEBUG_DESTROY_CONSOLE();

		openCMD_ = false;
	}
}

void Application::ActivateVSync(bool on)
{
	SDL_GL_SetSwapInterval(on);
}

APP::AppData Application::GetAppData()
{
	return data_;
}

bool Application::Initialize()
{
	// Load app init data
	JSON::ReadFile(ASSET::initDirectory_.c_str());

	CR_RJValue title = JSON::GetDocument()["Title"];
	CR_RJValue fullscreen = JSON::GetDocument()["Fullscreen"];
	CR_RJValue width = JSON::GetDocument()["Width"];
	CR_RJValue height = JSON::GetDocument()["Height"];
	CR_RJValue icon = JSON::GetDocument()["Icon"];

	if (title.IsString() && fullscreen.IsBool()
		&& width.IsInt() && height.IsInt()
		&& icon.IsString()) {

		data_.title.assign(title.GetString());
		data_.icon.assign(icon.GetString());
		data_.isFullscreen = fullscreen.GetBool();
		data_.width = width.GetInt();
		data_.height = height.GetInt();
	}

	else {
		jeDebugPrint("!Application - Wrong init data.\n");
		return false;
	}

	// Plant random seed
	Random::PlantSeed();

	// Initialize sdl info
	jeCheck(InitSDL());

	// Initialize opengl setting
	GLM::Resize(data_.width, data_.height);
	jeCheck(GLM::Init());

	// Generate built-in components
	jeCheck(ASSET::SetBuiltInComponents());

	// Load data from json files
	ASSET::LoadAssets();		

	// Set up imgui
	jeCheck(IMGUI::Init(pWindow_));
	IMGUI::AddEditorFunc(APP::EditorUpdate);
	IMGUI::AddEditorFunc(GLM::EditorUpdate);
	IMGUI::AddEditorFunc(STATE::EditorUpdate);

	// Load state info and bind systems here
	jeCheck(STATE::Init(pWindow_));

	return true;
}

void Application::Update()
{
	// Update the surface
	while (STATE::GetStatus()
		!= STATE::StateStatus::JE_STATE_QUIT)
		// Update state manager
		STATE::Update(&pEvent_);
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
	pWindow_ = SDL_CreateWindow(data_.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		data_.width, data_.height, SDL_WINDOW_OPENGL);

	if (!pWindow_) {
		jeDebugPrint("!Application - Window could not be created. SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	// Set window icon
	pIcon_ = IMG_Load(data_.icon.c_str());

	SDL_SetWindowIcon(pWindow_, pIcon_);

	// Get context
	pContext_ = SDL_GL_CreateContext(pWindow_);

	SDL_SetWindowFullscreen(pWindow_, data_.isFullscreen);

	// Get window surface
	pSurface_ = SDL_GetWindowSurface(pWindow_);

	// Fill the surface white
	SDL_FillRect(pSurface_, nullptr, SDL_MapRGB(pSurface_->format, 0xFF, 0xFF, 0xFF));

	return true;
}

void Application::CloseSDL()
{
	// Delete pIcon_
	SDL_FreeSurface(pIcon_);

	// Destroy
	SDL_GL_DeleteContext(pContext_);

	//Destroy window
	SDL_DestroyWindow(pWindow_);

	//Quit SDL subsystems
	SDL_Quit();
}

void Application::EditorUpdate(const float /*dt*/)
{
	// Basic debug window
	ImGui::Begin("Debug");
	ImGui::Text("*JEngine Frame Time: %.11f", STATE::frameTime_);
	ImGui::Text("*Resolution: %d x %d", data_.width, data_.height);

	if (!data_.isFullscreen) {
		ImGui::Text("*Screen Mode: Window Mode");
		if (ImGui::Button("Fullscreen")) {
			data_.isFullscreen = true;
			SDL_SetWindowFullscreen(pWindow_, data_.isFullscreen);
		}
	}

	else {
		ImGui::Text("*Screen Mode: Fullscreen");
		if (ImGui::Button("Window Mode")) {
			data_.isFullscreen = false;
			SDL_SetWindowFullscreen(pWindow_, data_.isFullscreen);
		}
	}

	ImGui::End();
}

jeEnd
