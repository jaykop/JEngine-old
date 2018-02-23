#pragma once
#include <vector>
#include "Timer.h"

union SDL_Event;
struct SDL_Window;

JE_BEGIN

class State;
class ObjectContainer;
using States = std::vector<State*>;

class StateManager {

	friend class State;
	friend class Application;
	friend class AssetManager;

	enum StateStatus  {
		STATE_NONE, STATE_RESTART, STATE_PAUSE, STATE_RESUME, STATE_CHANGE, STATE_RESUME_AND_CHANGE, STATE_QUIT };

public:

	static void Quit();
	static void Restart();
	static void Resume();
	static void Pause(const char* _nextState);
	static bool IsPaused();
	static void SetNextState(const char* _nextState);
	static void ResumeAndNext(const char* _nextState);
	static void SetStartingState(const char* _stateName);

	static StateStatus	GetStatus(void);
	static State*		GetCurrentState(void);
	static State*		GetState(const char* _stateName);
	static bool			HasState(const char* _stateName);

private:

	// Locked contsturctord and destructor
	StateManager() = delete;
	~StateManager() = delete;
	StateManager(const StateManager& /*_copy*/) = delete;
	void operator=(const StateManager& /*_copy*/) = delete;

	// Private member functions
	static void Init(SDL_Window* _pWindow);
	static void Update(SDL_Event* _event);
	static void Close();

	static void PushState(const char* _path, const char* _stateName);
	static void PopState(const char* _stateName);

	static void ChangeState();
	static void ClearStates();

	static void	EditorUpdate(const float _dt);

	// Private member variables
	static float		m_frameTime;
	static Timer		m_timer;
	static States		m_states;
	static StateStatus	m_status;
	static State*		m_pCurrent, *m_pNext;
	static SDL_Window*	m_pWindow;
	
#if defined(_DEBUG)
	static bool m_showUpdateMessage;
#endif // _DEBUG

};

using STATE = StateManager;

JE_END
