#pragma once
#include <vector>
#include "Timer.h"
#include "SDL2\SDL.h"

JE_BEGIN

class State;
using States = std::vector<State*>;

class StateManager {

	friend class Application;
	friend class AssetManager;
	enum StateStatus {
		S_NONE, S_PAUSE, S_RESUME, S_CHANGE, S_RESUME_AND_CHANGE, S_QUIT
	};

public:

	static void Quit();
	static void Resume();
	static void Pause(const char* _nextState);
	static void SetNextState(const char* _nextState);
	static void ResumeAndNext(const char* _nextState);
	static void SetStartingState(const char* _stateName);

	static StateStatus	GetStatus(void);
	static State*		GetCurrentState(void);
	static State*		GetState(const char* _stateName);

private:

	// Locked contsturctord and destructor
	StateManager() {};
	~StateManager() {};
	StateManager(const StateManager& /*_copy*/) = delete;
	void operator=(const StateManager& /*_copy*/) = delete;

	// Private member functions
	static void Init();
	static void Update(SDL_Event& _event);
	static void Close();

	static void PushState(const char* _path, const char* _stateName);
	static void PopState(const char* _stateName);

	static void ChangeState();
	static void ClearStates();

	// Private member variables
	static Timer		m_timer;
	static States		m_states;
	static StateStatus	m_status;
	static State*		m_pCurrent, *m_pNext, *m_pPause;

};

using STATE = StateManager;

JE_END
