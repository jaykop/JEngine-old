#pragma once
#include <vector>
#include "Timer.h"

union SDL_Event;
struct SDL_Window;

jeBegin

class State;
class ObjectContainer;
using States = std::vector<State*>;

class StateManager {

	friend class State;
	friend class Application;
	friend class AssetManager;

	enum StateStatus  {
		JE_STATE_NONE, JE_STATE_RESTART, JE_STATE_PAUSE, JE_STATE_RESUME, JE_STATE_CHANGE, JE_STATE_RESUME_AND_CHANGE, JE_STATE_QUIT };

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(StateManager)

public:

	static void Quit();
	static void Restart();
	static void Resume();
	static void Pause(const char* nextState);
	static bool IsPaused();
	static void SetNextState(const char* nextState);
	static void ResumeAndNext(const char* nextState);
	static void SetStartingState(const char* stateName);

	static StateStatus	GetStatus(void);
	static State*		GetCurrentState(void);
	static State*		GetState(const char* stateName);
	static bool			HasState(const char* stateName);
	static float		GetCurrentTime();
	static float		GetFrameRate();
	static unsigned		GetFramePerSecond();

private:

	// Private member functions
	static bool Init(SDL_Window* pWindow);
	static void Update(SDL_Event* pEvent);
	static void Close();

	static void PushState(const char* path, const char* stateName);
	static void PopState(const char* stateName);

	static void ChangeState();
	static void ClearStates();

	static void	EditorUpdate(float dt);

	// Private member variables
	static float		frameTime_;
	static unsigned		frames_;
	static Timer		timer_;
	static States		states_;
	static StateStatus	status_;
	static State*		pCurrent_, *pNext;
	static SDL_Window*	pWindow_;
	
#if defined(_DEBUG)
	static bool showUpdateMessage_;
#endif // _DEBUG

};

using STATE = StateManager;

jeEnd
