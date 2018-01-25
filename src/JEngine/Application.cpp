#include "GLManager.h"
#include "SDL_opengl.h"
#include "Application.h"
#include "StateManager.h"
#include "AssetManager.h"
#include "JsonParser.h"
#include "Random.h"

#ifdef JE_SUPPORT_IMGUI
#include "ImguiManager.h"
#endif

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

bool Application::Initialize()
{
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
		JE_DEBUG_PRINT("*Application: Wrong init data.\n");
		return false;
	}

	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// Print error message
		JE_DEBUG_PRINT("*Application: SDL could not initialize! SDL_Error - %s\n", SDL_GetError());
		return false;
	}

	// Call opengl 4.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	return true;
}

void Application::Update()
{	
	//SDL_SetVideoMode();
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	//Create window
	m_pWindow = SDL_CreateWindow(m_Data.m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_Data.m_width, m_Data.m_height, SDL_WINDOW_OPENGL);

	// Window flag, resolution
	//SDL_RenderSetLogicalSize();
	SDL_SetWindowFullscreen(m_pWindow, m_Data.m_isFullScreen);

	if (!m_pWindow) 
		JE_DEBUG_PRINT("*Application: Window could not be created! SDL_Error - %s\n", SDL_GetError());

	else {
		
		/*************** Random **************/
		Random::PlantSeed();

		/*************** OpenGL **************/
		m_pContext = SDL_GL_CreateContext(m_pWindow);
		GLM::initSDL_GL(float(m_Data.m_width), float(m_Data.m_height));

		/**************** imgui **************/
		#ifdef JE_SUPPORT_IMGUI
		IMGUI::Init(m_pWindow);
		#endif

		/**************** Built-in **************/
		ASSET::Load();	// Load info from json files
		STATE::Init();	// Bind systems here

		// Get window surface
		m_pSurface = SDL_GetWindowSurface(m_pWindow);

		// Fill the surface white
		SDL_FillRect(m_pSurface, nullptr, SDL_MapRGB(m_pSurface->format, 0xFF, 0xFF, 0xFF));

		// Update the surface
		while (STATE::GetStatus()
			!= STATE::StateStatus::S_QUIT) {
			
			// Update state manager
			STATE::Update(m_pEvent);
		
			// Update sdl window
			SDL_UpdateWindowSurface(m_pWindow);

		}	// while (STATE::GetStatus()
			// != STATE::StateStatus::S_QUIT) {

	} // else {
}

void Application::Close()
{
	STATE::Close();		// Remove systems and states
	ASSET::Unload();	// Clear loaded assets
	GLM::CloseSDL_GL();

	#ifdef JE_SUPPORT_IMGUI
	// Close imgui manager
	IMGUI::Close();
	#endif 
	
	// Destroy
	SDL_GL_DeleteContext(m_pContext);

	//Destroy window
	SDL_DestroyWindow(m_pWindow);

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Window* Application::GetWindow()
{
	return m_pWindow;
}

SDL_Surface* Application::GetSurface()
{
	return m_pSurface;
}

Application::InitData& Application::GetData()
{
	return m_Data;
}

JE_END
