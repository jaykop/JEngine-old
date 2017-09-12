#include "Application.h"
#include "StateManager.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Event				Application::m_pEvent;
SDL_Window*				Application::m_pWindow = nullptr;
Application::InitData	Application::m_pData = { "demo", false, 800, 600 };

Application::Application(const InitData& _data)
{
	m_pData = _data;
}

bool Application::Initialize()
{
	//Initialize SDL
	bool output = true;

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
	m_pWindow = SDL_CreateWindow(m_pData.m_title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_pData.m_width, m_pData.m_height, SDL_WINDOW_SHOWN);

	if (m_pWindow == nullptr) 
		JE_DEBUG_PRINT("Window could not be created! SDL_Error: %s\n", SDL_GetError());

	else {
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

		//Update the surface
		while (StateManager::GetStatus()
			!= StateManager::StateStatus::S_QUIT) {
			
			// Update state manager
			StateManager::Update(m_pEvent);
		
			// Update sdl window
			SDL_UpdateWindowSurface(m_pWindow);
		}

	} // else {
}

void Application::Close()
{
	// Close the state manager
	StateManager::Close();

	//Destroy window
	SDL_DestroyWindow(m_pWindow);

	//Quit SDL subsystems
	SDL_Quit();
}

NS_JE_END
