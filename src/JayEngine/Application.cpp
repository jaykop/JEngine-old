#include "GLManager.h"
#include "SDL2\SDL_opengl.h"
#include "Application.h"
#include "StateManager.h"
#include "AssetManager.h"
#include "ImguiManager.h"
#include "JsonParser.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Event		APP::m_pEvent;
SDL_Window*		APP::m_pWindow = nullptr;
SDL_Surface*	APP::m_pSurface = nullptr;
SDL_GLContext	APP::m_pContext = nullptr;
APP::InitData	APP::m_pData = { "demo", false, 800, 600 };

bool Application::Initialize(const InitData& _data)
{
	m_pData = _data;

	// Call opengl 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		// Print error message
		JE_DEBUG_PRINT("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void Application::Update()
{	
	//Create window
	m_pWindow = SDL_CreateWindow(m_pData.m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_pData.m_width, m_pData.m_height, SDL_WINDOW_OPENGL);

	if (!m_pWindow) 
		JE_DEBUG_PRINT("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	else {
		
		/*************** OpenGL **************/
		m_pContext = SDL_GL_CreateContext(m_pWindow);
		GLM::initSDL_GL();

		/**************** imgui **************/
		#ifdef JE_SUPPORT_IMGUI
		IMGUI::Init(m_pWindow);	
		#endif

		// Get window surface
		m_pSurface = SDL_GetWindowSurface(m_pWindow);

		// Fill the surface white
		SDL_FillRect(m_pSurface, nullptr, SDL_MapRGB(m_pSurface->format, 0xFF, 0xFF, 0xFF));

		// Load assets by asset manager
		// and init State manager
		ASSET::Load();
		STATE::Init();

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
	// Close and Unload 
	// the state manager and asset manager
	STATE::Close();
	ASSET::Unload();

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
	return m_pData;
}

JE_END
