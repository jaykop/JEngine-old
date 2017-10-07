#include "GLManager.h"
#include "SDL2\SDL_opengl.h"
#include "Application.h"
#include "StateManager.h"
#include "ImguiManager.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Event				Application::m_pEvent;
SDL_Window*				Application::m_pWindow = nullptr;
SDL_GLContext			Application::m_pContext = nullptr;
Application::InitData	Application::m_pData = { "demo", false, 800, 600 };

Application::Application(const InitData& _data)
{
	m_pData = _data;
}

bool Application::Initialize()
{
	//Initialize SDL
	bool output = true;

	// Call opengl 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Check right init
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		// Print error message
		JE_DEBUG_PRINT("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		output = false;
	}

	return output;
}

void Application::Update()
{	

	//The surface contained by the window
	SDL_Surface* screenSurface = nullptr;

	//Create window
	m_pWindow = SDL_CreateWindow(m_pData.m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		m_pData.m_width, m_pData.m_height, SDL_WINDOW_OPENGL);

	if (m_pWindow == nullptr) 
		JE_DEBUG_PRINT("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	else {
		
		/*************** OpenGL **************/
		m_pContext = SDL_GL_CreateContext(m_pWindow);	// Get GL context
		GLManager::initSDL_GL();						// Init gl

		// TODO
		GLManager::ActivateShader(						// Call Shader
			"../../src/Shader/vertexshader.vs",				// vertex Shader
			"../../src/Shader/fragmentshader.fs");			// fragment shader

		/**************** imgui **************/
		//ImguiManager::Init(m_pWindow);					// init imgui

		//Get window surface
		screenSurface = SDL_GetWindowSurface(m_pWindow);

		//Fill the surface white
		SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

		//TODO
		// Making new state
		StateManager::PushState("testState1");
		StateManager::PushState("testState2");
		StateManager::PushState("testState3");
		StateManager::PushState("PauseState");
		StateManager::SetStartingState("testState1");

		// Init State manager
		StateManager::Init();

		// Update the surface
		while (StateManager::GetStatus()
			!= StateManager::StateStatus::S_QUIT) {
			
			// Update state manager
			StateManager::Update(m_pEvent);
		
			// Update sdl window
			SDL_UpdateWindowSurface(m_pWindow);

		}	// while (StateManager::GetStatus()
			// != StateManager::StateStatus::S_QUIT) {

	} // else {
}

void Application::Close()
{
	// Close the state manager
	StateManager::Close();

	// Close imgui manager
	//ImguiManager::Close();

	// Destroy
	SDL_GL_DeleteContext(m_pContext);

	//Destroy window
	SDL_DestroyWindow(m_pWindow);

	//Quit SDL subsystems
	SDL_Quit();
}

SDL_Window * Application::GetWindow()
{
	return m_pWindow;
}

Application::InitData& Application::GetData()
{
	return m_pData;
}

NS_JE_END
