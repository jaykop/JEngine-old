#include "State.h"
#include "StateManager.h"
#include "InputHandler.h"
#include "Application.h"
#include "SystemManager.h"
#include "ImguiManager.h"
#include "AssetManager.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
Timer						StateManager::m_timer;
States						StateManager::m_states;
StateManager::StateStatus	StateManager::m_status = S_CHANGE;
State						*StateManager::m_pCurrent = nullptr, 
							*StateManager::m_pNext = nullptr, 
							*StateManager::m_pPause = nullptr;

//////////////////////////////////////////////////////////////////////////
// funciton bodues
//////////////////////////////////////////////////////////////////////////
void StateManager::Load()
{
	
}

void StateManager::Init()
{
	//TODO
	// Making new state
	StateManager::PushState("testState1");
	StateManager::PushState("testState2");
	StateManager::PushState("testState3");
	StateManager::PushState("PauseState");
	StateManager::SetStartingState("testState1");

	// Allocate systems in advance
	SystemManager::Bind();
}

void StateManager::Update(SDL_Event& _event)
{
	//	Get pointer to window
	SDL_Window* pWindow = Application::GetWindow();

	// Timer
	m_timer.Start();

	static int s_frame = 0;
	static float s_dt = 0.f, s_stack = 0.f;

	ChangeState();

	while (SDL_PollEvent(&_event) != 0	// Event handler loop
		|| m_status == S_NONE) {		// State updating loop

		//Get input by input handler
		InputHandler::Update(&_event);

		// Update state manager
		m_pCurrent->Update(s_dt);

		s_dt = m_timer.GetTime();	// Get delta time
		++s_frame;					// Increment to framw
		s_stack += s_dt;			// Stack dt

		if (s_stack >= 1.f) {		// Refresh every sec
			s_stack = 0.f;
			s_frame = 0;
			m_timer.Start();

			// Swap buffer
			SDL_GL_SwapWindow(pWindow);

			// Update imgui renderer
			#ifdef JE_SUPPORT_IMGUI
			ImguiManager::Update();
			#endif
		}
	}

	switch (m_status) {

		// TODO 
		// Pause process
	case S_PAUSE:
		SystemManager::Pause();
		break;

	case S_QUIT:				// The case to quit app
		while (m_pCurrent) {
			State* pLast = m_pCurrent->m_pLastStage;
			m_pCurrent->Close();
			m_pCurrent->Unload();
			m_pCurrent = pLast;
		}
		break;
	
	case S_RESUME:				// The case to resume to last state
		m_pCurrent->Close();
		m_pCurrent->Unload();
		SystemManager::Resume();
		break;

	case S_CHANGE:				// The case to change to next state
	case S_RESUME_AND_CHANGE:	// The case to resume and change
		m_pCurrent->Close();
		m_pCurrent->Unload();
		break;
	}
}

void StateManager::Close()
{
	// Deallocate systems in advance
	SystemManager::Unbind();

	// Clear all the states
	ClearStates();
}

void StateManager::Unload()
{
	AssetManager::Unload();
}

void StateManager::ChangeState()
{
	// Load and init agein
	// for next stage
	if (m_status == S_CHANGE || m_status == S_PAUSE) {
		
		// Save state to resume
		if (m_status == S_PAUSE) {
			State* toResume = m_pCurrent;
			m_pCurrent = m_pNext;
			m_pCurrent->m_pLastStage = toResume;
		}

		// Just change current state
		else
			m_pCurrent = m_pNext;

		// Renew the state
		m_pCurrent->Load();
		m_pCurrent->Init();
	}

	// Resume state
	else if (m_status == S_RESUME) {
		State* release = m_pCurrent;
		m_pCurrent = m_pNext = m_pCurrent->m_pLastStage;
		release->m_pLastStage = nullptr;
	}

	// Resume and change
	else if (m_status == S_RESUME_AND_CHANGE) {
		m_pCurrent = m_pCurrent->m_pLastStage;
		m_status = S_CHANGE;
	}

	// Refresh the status
	if (m_pCurrent == m_pNext)
		m_status = S_NONE;
}

void StateManager::PushState(const char* _stateName)
{
	bool sameOne = false;
	for (auto it = m_states.begin();
		it != m_states.end(); ++it)
	{
		// If there is already same one,
		// mark toggle
		if (!strcmp((*it)->m_name.c_str(), _stateName)) {
			sameOne = true;
			break;
		}
	}	

	// Prevent duplicated state
	if (!sameOne) {

		// Make new one
		State* newState = new State(_stateName);

		// Push to the vector
		m_states.push_back(newState);
	}

	// If there is same state already,
	// do not make anything
	else
		JE_DEBUG_PRINT("Same state exist already!\n");
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
	m_status = S_QUIT;
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
					JE_DEBUG_PRINT("Same state!\n");

				// Found the state
				else {
					m_pNext = it;
					m_status = S_CHANGE;
				}

				break;
			}
		}

		if (!found)
			JE_DEBUG_PRINT("No such state!\n");
	}

	else
		JE_DEBUG_PRINT("Cannot move on paused situation!\n");
}

void StateManager::Pause(const char* _nextState)
{
	// Set state to pause
	m_pNext = GetState(_nextState);
	m_status = S_PAUSE;
}

void StateManager::Resume()
{
	// Check state to resume
	if (m_pCurrent->m_pLastStage)
		m_status = S_RESUME;

	else
		JE_DEBUG_PRINT("Cannot resume!\n");
}

void StateManager::ResumeAndNext(const char* _nextState)
{
	m_status = S_RESUME_AND_CHANGE;
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
	JE_DEBUG_PRINT("No such state!\n");
	return nullptr;
}

void StateManager::ClearStates()
{
	// Remove all states from the vector
	for (auto it = m_states.begin(); it != m_states.end(); )
		delete (*it++);	

	m_states.clear();
}

JE_END


