#pragma once
#include <vector>
#include "Macro.h"
#include "SDL2\SDL.h"

NS_JE_BEGIN

class State;
typedef std::vector<State*> States;

class StateManager {

public:

	enum StateStatus {
		S_NONE, S_PAUSE, S_RESUME, S_CHANGE, S_RESUME_AND_CHANGE, S_QUIT
	};

	//static void Load();
	static void Init();
	static void Update(SDL_Event& _event);
	static void Close();
	//static void Unload();

	static void PushState(const char* _stateName);
	static void PopState(const char* _stateName);

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

	StateManager() {};
	~StateManager() {};
	StateManager(const StateManager& /*_copy*/) {};
	void operator=(const StateManager& /*_copy*/) {};

	static void EventHandle();
	static void ChangeState();

	static void Clear();

	static States		m_states;
	static StateStatus	m_status;
	static State*		m_pCurrent, *m_pNext, *m_pPause;

};

NS_JE_END