#include "SDL.h"
#include "State.h"
#include "StateManager.h"
#include "InputHandler.h"
#include "SystemManager.h"
#include "ImguiManager.h"
#include "AssetManager.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Window*					StateManager::m_pWindow = nullptr;
Timer						StateManager::m_timer;
States						StateManager::m_states;
StateManager::StateStatus	StateManager::m_status = STATE_CHANGE;
ObjectContainer				*StateManager::m_pOBC = nullptr;
State						*StateManager::m_pCurrent = nullptr, 
							*StateManager::m_pNext = nullptr;

#if defined(_DEBUG)
bool						StateManager::m_showUpdateMessage = true;
#endif // _DEBUG

//////////////////////////////////////////////////////////////////////////
// funciton bodues
//////////////////////////////////////////////////////////////////////////
void StateManager::Init(SDL_Window* _pWindow)
{
	if (_pWindow) {
		m_pWindow = _pWindow;

		// Allocate systems in advance
		SYSTEM::Bind();
	}

	else
		JE_DEBUG_PRINT("!StateManager: Window pointer is null.\n");
}

void StateManager::Update(SDL_Event* _event)
{
	// Timer
	m_timer.Start();

	static float s_dt = 1.f / 60.f, s_stack, s_frameTime, s_newTime, s_currentTime;
	s_stack = s_frameTime = s_newTime = s_currentTime = 0.f;

	ChangeState();

	while (SDL_PollEvent(_event) != 0	// Event handler loop
		|| m_status == STATE_NONE) {	// State updating loop

		INPUT::Update(_event);			// Get input by input handler
		IMGUI::EventUpdate(_event);		// Get input by imgui manager
		
		s_newTime = m_timer.GetTime();	// Get delta time
		s_frameTime						// Get frame time
			= s_newTime - s_currentTime;

		if (s_frameTime > 0.25f)		// Lock the frame time
			s_frameTime = 0.25f;

		s_currentTime = s_newTime;		// Refresh current time
		s_stack += s_frameTime;			// Stack frame time

		// Fixed timestep
		if (s_stack >= s_dt) {			// Refresh every sec
			
			m_pCurrent->Update(s_dt);	// Update state
			IMGUI::Update(s_frameTime);	// Update imgui renderer
			
			/* Update rendrer with physics together 
			so makes rendering scene more smoothly */
			SDL_GL_SwapWindow(m_pWindow);
			s_stack -= s_dt;
		}
	}

	switch (m_status) {

	// TODO 
	// Pause process
	case STATE_PAUSE:
		SYSTEM::Pause();
		break;

	case STATE_QUIT:				// The case to quit app
		while (m_pCurrent) {
			State* pLast = m_pCurrent->m_pLastStage;
			m_pCurrent->Close();
			m_pCurrent->Unload();
			m_pCurrent = pLast;
		}
		break;
	
	case STATE_RESUME:				// The case to resume to last state
		m_pCurrent->Close();
		m_pCurrent->Unload();
		SYSTEM::Resume();
		break;

	case STATE_RESTART:				// The case to restart the current state
	case STATE_CHANGE:				// The case to change to next state
	case STATE_RESUME_AND_CHANGE:	// The case to resume and change
		m_pCurrent->Close();
		m_pCurrent->Unload();
		break;
	}
}

void StateManager::Close()
{
	// Deallocate systems in advance
	SYSTEM::Unbind();

	// Clear all the states
	ClearStates();
}

void StateManager::ChangeState()
{
	// Load and init agein
	// for next stage
	if (m_status == STATE_CHANGE 
		|| m_status == STATE_PAUSE
		|| m_status == STATE_RESTART) {
		
		// Save state to resume
		if (m_status == STATE_PAUSE) {
			State* toResume = m_pCurrent;
			m_pCurrent = m_pNext;
			m_pCurrent->m_pLastStage = toResume;
		}

		// Just change current state
		else if (m_status == STATE_CHANGE)
			m_pCurrent = m_pNext;

		// Renew the state
		m_pCurrent->Load();
		m_pCurrent->Init();
	}

	// Resume state
	else if (m_status == STATE_RESUME) {
		State* release = m_pCurrent;
		m_pCurrent = m_pNext = m_pCurrent->m_pLastStage;
		m_pOBC = m_pCurrent->m_objContainer;
		release->m_pLastStage = nullptr;
	}

	// Resume and change
	else if (m_status == STATE_RESUME_AND_CHANGE) {
		m_pCurrent = m_pCurrent->m_pLastStage;
		m_pOBC = m_pCurrent->m_objContainer;
		m_status = STATE_CHANGE;
	}

	// Refresh the status
	if (m_pCurrent == m_pNext)
		m_status = STATE_NONE;
}

void StateManager::PushState(const char* _path, const char* _stateName)
{
	bool sameOne = false;
	for (auto it = m_states.begin();
		it != m_states.end(); ++it) {
		// If there is already same one,
		// mark toggle
		if (!strcmp((*it)->m_name.c_str(), _stateName)) {
			sameOne = true;
			JE_DEBUG_PRINT("!StateManager - Existing state: %s\n", _stateName);
			break;
		}
	}	

	// Prevent duplicated state
	if (!sameOne) {

		// Make new one
		State* newState = new State(_stateName);
		newState->m_loadDirectory.assign(_path);

		// Push to the vector
		m_states.push_back(newState);
		if (m_states.size() == 1)
			SetStartingState(_stateName);
	}
}

void StateManager::PopState(const char* _stateName)
{
	// Find where it is
	for (auto it = m_states.begin();
		it != m_states.end(); ++it) {

		// Get same name state
		if (!strcmp((*it)->m_name.c_str(), _stateName)) {
			delete (*it);		// Remove the memory
			m_states.erase(it);	// Remove form the vector
			break;
		}
	}
}

void StateManager::SetStartingState(const char * _stateName)
{
	// Find where it is
	for (auto it = m_states.begin();
		it != m_states.end(); ++it) {

		// Get same name state
		if (!strcmp((*it)->m_name.c_str(), _stateName))
			m_pNext = m_pCurrent = (*it);
	}
}

void StateManager::Quit()
{
	m_status = STATE_QUIT;
}

void StateManager::Restart()
{
	if (IsPaused())
		JE_DEBUG_PRINT("!StateManager - Cannot restart on pause state.\n");
	else
		m_status = STATE_RESTART;
}

bool StateManager::IsPaused()
{
	return m_pCurrent->m_pLastStage != nullptr ? true : false ;
}

StateManager::StateStatus StateManager::GetStatus(void)
{
	return m_status;
}

void StateManager::SetNextState(const char* _stateName)
{
	// Check if there is no state to resume
	if (!m_pCurrent->m_pLastStage) {
		bool found = false;
		for (auto it : m_states) {

			// If found the one,
			if (!strcmp(_stateName, it->m_name.c_str())) {

				found = true;

				// Current state is the state
				if (m_pCurrent == it)
					JE_DEBUG_PRINT("!StateManager - As same as current state: %s\n", _stateName);

				// Found the state
				else {
					m_pNext = it;
					m_status = STATE_CHANGE;
				}

				break;
			}
		}

		if (!found)
			JE_DEBUG_PRINT("!StateManager - No such name of enrolled state: %s\n", _stateName);
	}

	else
		JE_DEBUG_PRINT("!StateManager - Cannot move on paused status.\n");
}

void StateManager::Pause(const char* _nextState)
{
	// Set state to pause
	m_pNext = GetState(_nextState);
	m_status = STATE_PAUSE;
}

void StateManager::Resume()
{
	// Check state to resume
	if (m_pCurrent->m_pLastStage)
		m_status = STATE_RESUME;

	else
		JE_DEBUG_PRINT("!StateManager - No state to resume.\n");
}

void StateManager::ResumeAndNext(const char* _nextState)
{
	m_status = STATE_RESUME_AND_CHANGE;
	m_pNext = GetState(_nextState);
}

State* StateManager::GetCurrentState(void)
{
	return m_pCurrent;
}

State* StateManager::GetState(const char* _stateName)
{
	// Find the same name 
	for (auto it : m_states)
		if (!strcmp(_stateName, it->m_name.c_str()))
			return it;

	// If there is no,
	JE_DEBUG_PRINT("!StateManager - No such name of enrolled state: %s\n", _stateName);
	return nullptr;
}

ObjectContainer* StateManager::GetContainer()
{
	return m_pOBC;
}

void StateManager::ClearStates()
{
	// Remove all states from the vector
	for (auto it = m_states.begin(); it != m_states.end(); )
		delete (*it++);	

	m_states.clear();
}

void StateManager::SetContainer(ObjectContainer * _container)
{
	m_pOBC = _container;
}

JE_END


