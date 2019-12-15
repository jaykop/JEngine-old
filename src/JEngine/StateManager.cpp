#include "SDL.h"
#include "State.h"
#include "StateManager.h"
#include "InputHandler.h"
#include "SystemManager.h"
#include "ObjectContainer.h"
#include "imgui.h"
#include "ImguiManager.h"

#if _DEBUG
#include "Application.h"
#endif

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
#if defined(_DEBUG)
bool			    STATE::showUpdateMessage_ = true;
#endif // _DEBUG

SDL_Window*		    STATE::pWindow_ = nullptr;
Timer			    STATE::timer_;
States			    STATE::states_;
STATE::StateStatus	STATE::status__ = JE_STATE_CHANGE;
State			    *STATE::pCurrent_ = nullptr,
					*STATE::pNext = nullptr;
float			    STATE::frameTime_ = 0.f;
unsigned			STATE::frames_ = 0;
std::string			STATE::firstState_;

//////////////////////////////////////////////////////////////////////////
// funciton bodues
//////////////////////////////////////////////////////////////////////////
bool StateManager::Init(SDL_Window* pWindow)
{
    if (pWindow) {
        pWindow_ = pWindow;

        INPUT::Init();	// Init input keys
        SYSTEM::Bind();	// Allocate systems in advance
        return true;
    }

    jeDebugPrint("!StateManager: Window pointer is null.\n");
    return false;

}

void StateManager::Update(SDL_Event* pEvent)
{
    timer_.Start();

    static float s_sec = 1.f, s_stack, s_newTime, s_currentTime;
    s_stack = s_newTime = s_currentTime = 0.f;

    ChangeState();

    while (SDL_PollEvent(pEvent) != 0	// Event handler loop
        || status__ == JE_STATE_NONE) {	// State updating loop

        IMGUI::EventUpdate(pEvent);	// Get input by imgui manager
        INPUT::Update(pEvent);		// Get input by input handler

        s_newTime = timer_.GetTime();	// Get delta time
        frameTime_						// Get frame time
            = s_newTime - s_currentTime;

        if (frameTime_ > 0.25f)	// Lock the frame time
            frameTime_ = 0.25f;

        s_stack += frameTime_;		// Stack frame time
		frames_++;					// Stack frame

        // Fixed timestep
        if (s_stack >= s_sec) {		// Refresh every sec

			s_currentTime = s_newTime;	// Refresh current time

            pCurrent_->Update(frameTime_);// Update state
            IMGUI::Update(frameTime_);		// Update imgui renderer

			// Reset mouse wheel session
			INPUT::mouseWheel_ = 0;

            /* Update rendrer with physics together
            so makes rendering scene more smoothly */
            SDL_GL_SwapWindow(pWindow_);

#if _DEBUG
			static std::string s_fps;
			s_fps.assign(APP::GetAppData().title + " [fps: " + std::to_string(frames_) + "]");
			SDL_SetWindowTitle(pWindow_, s_fps.c_str());
#endif
			s_stack = 0.f;	//s_stack -= s_dt;
			frames_ = 0;
        }
    }

    switch (status__) {

		// Keep updaring
	case JE_STATE_NONE:
	default:
		break;

        // Pause process
    case JE_STATE_PAUSE:
        SYSTEM::Pause();
        break;

    case JE_STATE_QUIT:				// The case to quit app
        while (pCurrent_) {
            State* pLast = pCurrent_->pLastStage_;
            pCurrent_->Close();
            pCurrent_->Unload();
            pCurrent_ = pLast;
        }
        break;

    case JE_STATE_RESUME:				// The case to resume to last state
        pCurrent_->Close();
        pCurrent_->Unload();
        SYSTEM::Resume();
        break;

    case JE_STATE_RESTART:				// The case to restart the current state
    case JE_STATE_CHANGE:				// The case to change to next state
    case JE_STATE_RESUME_AND_CHANGE:	// The case to resume and change
        pCurrent_->Close();
        pCurrent_->Unload();
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
    if (status__ == JE_STATE_CHANGE
        || status__ == JE_STATE_PAUSE
        || status__ == JE_STATE_RESTART) {

        // Save state to resume
        if (status__ == JE_STATE_PAUSE) {
            State* toResume = pCurrent_;
            pCurrent_ = pNext;
            pCurrent_->pLastStage_ = toResume;
        }

        // Just change current state
        else if (status__ == JE_STATE_CHANGE)
            pCurrent_ = pNext;

        // Renew the state
        pCurrent_->Load();
        pCurrent_->Init();

        // Add object container editor function
        static bool s_addContainerEditor = false;
        if (!s_addContainerEditor) {
            IMGUI::AddEditorFunc(OBJECT::EditorUpdate);
            s_addContainerEditor = true;
        }
    }

    // Resume state
    else if (status__ == JE_STATE_RESUME) {
        State* release = pCurrent_;
        pCurrent_ = pNext = pCurrent_->pLastStage_;
		OBJECT::pContainer_ = pCurrent_->pObjContainer_;
        release->pLastStage_ = nullptr;
    }

    // Resume and change
    else if (status__ == JE_STATE_RESUME_AND_CHANGE) {
        pCurrent_ = pCurrent_->pLastStage_;
		OBJECT::pContainer_ = pCurrent_->pObjContainer_;
        status__ = JE_STATE_CHANGE;
    }

    // Refresh the status__
    if (pCurrent_ == pNext)
        status__ = JE_STATE_NONE;
}

void StateManager::PushState(const char* path, const char* stateName)
{
    bool sameOne = false;
    for (auto it = states_.begin();
        it != states_.end(); ++it) {
        // If there is already same one,
        // mark toggle
        if (!strcmp((*it)->name_.c_str(), stateName)) {
            sameOne = true;
            jeDebugPrint("!StateManager - Existing state: %s\n", stateName);
            break;
        }
    }

    // Prevent duplicated state
    if (!sameOne) {

        // Make new one
        State* newState = new State(stateName);
        newState->loadDirectory_.assign(path);

        // Push to the vector
        states_.push_back(newState);
        if (states_.size() == 1)
            SetStartingState(stateName);
    }
}

void StateManager::PopState(const char* stateName)
{
    // Find where it is
    for (auto it = states_.begin();
        it != states_.end(); ++it) {

        // Get same name state
        if (!strcmp((*it)->name_.c_str(), stateName)) {
            delete (*it);		// Remove the memory
            states_.erase(it);	// Remove form the vector
            break;
        }
    }
}

void StateManager::SetStartingState(const char * stateName)
{
	if (firstState_.empty())
		firstState_.assign(stateName);

	// Find where it is
	for (auto it = states_.begin();
		it != states_.end(); ++it) {

		// Get same name state
		if (!strcmp((*it)->name_.c_str(), firstState_.c_str()))
			pNext = pCurrent_ = (*it);
	}

}

void StateManager::Quit()
{
    status__ = JE_STATE_QUIT;
}

void StateManager::Restart()
{
    if (IsPaused())
        jeDebugPrint("!StateManager - Cannot restart on pause state.\n");
    else
        status__ = JE_STATE_RESTART;
}

bool StateManager::IsPaused()
{
    return pCurrent_->pLastStage_ != nullptr;
}

StateManager::StateStatus StateManager::GetStatus(void)
{
    return status__;
}

void StateManager::SetNextState(const char* nextState)
{
    // Current state is the state
    if (!strcmp(pCurrent_->name_.c_str(), nextState))
        jeDebugPrint("!StateManager - As same as current state: %s\n", nextState);

    else {
        // Check if there is no state to resume
        if (!pCurrent_->pLastStage_) {

            if (HasState(nextState)) {
                pNext = GetState(nextState);
                status__ = JE_STATE_CHANGE;
            }
        }

        else
            jeDebugPrint("!StateManager - Cannot move on paused status__.\n");
    }
}

void StateManager::Pause(const char* nextState)
{
    // Current state is the state
    if (!strcmp(pCurrent_->name_.c_str(), nextState))
        jeDebugPrint("!StateManager - As same as current state: %s\n", nextState);

    else if (HasState(nextState)) {
        pNext = GetState(nextState);
        status__ = JE_STATE_PAUSE;
    }

}

void StateManager::Resume()
{
    // Check state to resume
    if (pCurrent_->pLastStage_)
        status__ = JE_STATE_RESUME;

    else
        jeDebugPrint("!StateManager - No state to resume.\n");
}

void StateManager::ResumeAndNext(const char* nextState)
{
    if (HasState(nextState)) {
        status__ = JE_STATE_RESUME_AND_CHANGE;
        pNext = GetState(nextState);
    }
}

State* StateManager::GetCurrentState(void)
{
    return pCurrent_;
}

State* StateManager::GetState(const char* stateName)
{
    // Find the same name 
    for (auto it : states_)
        if (!strcmp(stateName, it->name_.c_str()))
            return it;

    // If there is no,
    jeDebugPrint("!StateManager - No such name of enrolled state: %s\n", stateName);
    return nullptr;
}

bool StateManager::HasState(const char *stateName)
{
    bool found = false;
    for (auto state : states_) {

        // If found the one,
        if (!strcmp(stateName, state->name_.c_str())) {
            found = true;
            return found;
        }
    }

    if (!found)
        jeDebugPrint("!StateManager - No such name of enrolled state: %s\n", stateName);

    return found;
}

float StateManager::GetCurrentTime()
{
    return timer_.GetTime();
}

unsigned StateManager::GetFramePerSecond()
{
	return frames_;
}

float StateManager::GetFrameRate()
{
	return frameTime_;
}

void StateManager::ClearStates()
{
    // Remove all states from the vector
    for (auto it = states_.begin(); it != states_.end(); )
        delete (*it++);

    states_.clear();
}

void StateManager::EditorUpdate(const float /*dt*/)
{
    static bool foundObject = false, showLevels = false;

    ImGui::Begin("StateManager");
    ImGui::Text("*Current State: %s", pCurrent_->name_.c_str());

    ImGui::Text("*Total States: %d", states_.size());
    ImGui::Text("*States Stacked: %d", SYSTEM::pauseStack_.size());

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
        for (auto state : states_)
            ImGui::Text("%s", state->name_.c_str());
        ImGui::End();
    }
}

jeEnd


