#include "SDL.h"
#include "State.h"
#include "StateManager.h"
#include "InputHandler.h"
#include "SystemManager.h"
#include "ObjectContainer.h"
#include "imgui.h"
#include "ImguiManager.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SDL_Window*			STATE::m_pWindow = nullptr;
Timer				STATE::m_timer;
States				STATE::m_states;
STATE::StateStatus	STATE::m_status = STATE_CHANGE;
State				*STATE::m_pCurrent = nullptr,
					*STATE::m_pNext = nullptr;
float				STATE::m_frameTime = 0.f;

#if defined(_DEBUG)
bool				STATE::m_showUpdateMessage = true;
#endif // _DEBUG

//////////////////////////////////////////////////////////////////////////
// funciton bodues
//////////////////////////////////////////////////////////////////////////
bool StateManager::Init(SDL_Window* _pWindow)
{
	if (_pWindow) {
		m_pWindow = _pWindow;

		INPUT::Init();	// Init input keys
		SYSTEM::Bind();	// Allocate systems in advance
		return true;
	}

	jeDebugPrint("!StateManager: Window pointer is null.\n");
	return false;
	
}

void StateManager::Update(SDL_Event* _event)
{
	m_timer.Start();

	static float s_dt = 1.f / 60.f, s_stack, s_newTime, s_currentTime;
	s_stack = s_newTime = s_currentTime = 0.f;

	ChangeState();

	while (SDL_PollEvent(_event) != 0	// Event handler loop
		|| m_status == STATE_NONE) {	// State updating loop

		IMGUI::EventUpdate(_event);		// Get input by imgui manager
		INPUT::Update(_event);			// Get input by input handler

		s_newTime = m_timer.GetTime();	// Get delta time
		m_frameTime						// Get frame time
			= s_newTime - s_currentTime;

		if (m_frameTime > 0.25f)		// Lock the frame time
			m_frameTime = 0.25f;

		s_currentTime = s_newTime;		// Refresh current time
		s_stack += m_frameTime;			// Stack frame time

		// Fixed timestep
		if (s_stack >= s_dt) {			// Refresh every sec
			
			INPUT::m_mouseWheel = 0;	// Reset mouse wheel session
			m_pCurrent->Update(s_dt);	// Update state
			IMGUI::Update(s_dt);		// Update imgui renderer
			
			/* Update rendrer with physics together 
			so makes rendering scene more smoothly */
			SDL_GL_SwapWindow(m_pWindow);
			s_stack -= s_dt;
		}
	}

	switch (m_status) {

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

		// Add object container editor function
		static bool s_addContainerEditor = false;
		if (!s_addContainerEditor) {
			IMGUI::AddEditorFunc(OBJECT::EditorUpdate);
			s_addContainerEditor = true;
		}
	}

	// Resume state
	else if (m_status == STATE_RESUME) {
		State* release = m_pCurrent;
		m_pCurrent = m_pNext = m_pCurrent->m_pLastStage;
		OBJECT::m_pSharedContainer = m_pCurrent->m_objContainer;
		release->m_pLastStage = nullptr;
	}

	// Resume and change
	else if (m_status == STATE_RESUME_AND_CHANGE) {
		m_pCurrent = m_pCurrent->m_pLastStage;
		OBJECT::m_pSharedContainer = m_pCurrent->m_objContainer;
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
			jeDebugPrint("!StateManager - Existing state: %s\n", _stateName);
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
		jeDebugPrint("!StateManager - Cannot restart on pause state.\n");
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

void StateManager::SetNextState(const char* _nextState)
{
	// Current state is the state
	if (!strcmp(m_pCurrent->m_name.c_str(), _nextState))
		jeDebugPrint("!StateManager - As same as current state: %s\n", _nextState);

	else {
		// Check if there is no state to resume
		if (!m_pCurrent->m_pLastStage) {
			
			if (HasState(_nextState)) {
				m_pNext = GetState(_nextState);
				m_status = STATE_CHANGE;
			}
		}

		else
			jeDebugPrint("!StateManager - Cannot move on paused status.\n");
	}
}

void StateManager::Pause(const char* _nextState)
{
	// Current state is the state
	if (!strcmp(m_pCurrent->m_name.c_str(), _nextState))
		jeDebugPrint("!StateManager - As same as current state: %s\n", _nextState);

	else if (HasState(_nextState)) {
		m_pNext = GetState(_nextState);
		m_status = STATE_PAUSE;
	}
	
}

void StateManager::Resume()
{
	// Check state to resume
	if (m_pCurrent->m_pLastStage)
		m_status = STATE_RESUME;

	else
		jeDebugPrint("!StateManager - No state to resume.\n");
}

void StateManager::ResumeAndNext(const char* _nextState)
{
	if(HasState(_nextState)) {
		m_status = STATE_RESUME_AND_CHANGE;
		m_pNext = GetState(_nextState);
	}
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
	jeDebugPrint("!StateManager - No such name of enrolled state: %s\n", _stateName);
	return nullptr;
}

bool StateManager::HasState(const char *_stateName)
{
	bool found = false;
	for (auto state : m_states) {

		// If found the one,
		if (!strcmp(_stateName, state->m_name.c_str())) {
			found = true;
			return found;
		}
	}

	if (!found)
		jeDebugPrint("!StateManager - No such name of enrolled state: %s\n", _stateName);

	return found;
}

float StateManager::GetCurrentTime()
{
	return m_timer.GetTime();
}

void StateManager::ClearStates()
{
	// Remove all states from the vector
	for (auto it = m_states.begin(); it != m_states.end(); )
		delete (*it++);	

	m_states.clear();
}

void StateManager::EditorUpdate(const float /*_dt*/)
{
	static bool foundObject = false, showLevels = false;

	ImGui::Begin("StateManager");
	ImGui::Text("*Current State: %s", m_pCurrent->m_name.c_str());

	ImGui::Text("*Total States: %d", m_states.size());
	ImGui::Text("*States Stacked: %d", SYSTEM::m_pauseStack.size());

	if (ImGui::Button("Show Level List"))
		showLevels = !showLevels;

	static char s_StateId[128] = "StateName";
	ImGui::InputText("", s_StateId, IM_ARRAYSIZE(s_StateId));

	ImGui::SameLine();
	if (ImGui::Button("Go"))
		SetNextState(s_StateId);

	ImGui::SameLine();
	ImGui::Text("/");

	ImGui::SameLine();
	if (ImGui::Button("Pause"))
		Pause(s_StateId);

	if (ImGui::Button("Restart"))
		Restart();

	ImGui::SameLine();
	if (ImGui::Button("Resume"))
		Resume();

	ImGui::SameLine();
	if (ImGui::Button("Quit"))
		Quit();

	ImGui::End();

	if (showLevels)
	{
		ImGui::Begin("LevelList");
		for (auto state : m_states)
			ImGui::Text("%s", state->m_name.c_str());
		ImGui::End();
	}
}

jeEnd


