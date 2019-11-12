/******************************************************************************/
/*!
\file   scene_manager.cpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/02(yy/mm/dd)

\description
Contains the methods of SceneManager class
*/
/******************************************************************************/

#include <scene_manager.hpp>
#include <timer.hpp>
#include <scene.hpp>
#include <input_handler.hpp>
#include <gl_manager.hpp>
#include <SDL.h>

jeBegin

// constant value
const float SECOND = 1.f, MAX_FRAME_TIME = 0.25f;

// Initialize the static variables
Timer SceneManager::timer_;
int SceneManager::frames_ = 0;
float SceneManager::frameTime_ = 0.f;
SDL_Window* SceneManager::window_ = nullptr;
Scene* SceneManager::currentScene_ = nullptr;
SceneManager::SceneStatus SceneManager::status_ = SceneManager::SceneStatus::JE_STATE_NONE;

bool SceneManager::initialize(SDL_Window* window)
{
	if (window) {
		
		window_ = window;
		
		// Initialize input system
		
		// 

		// bind current state's system
		Scene::bind_system();
		
		return true;
	}

	return false;
}

void SceneManager::update(SDL_Event* event)
{
	static float timeStack, elapsedTime, currentTime;
	timeStack = elapsedTime = currentTime = 0.f;

	timer_.start();
	change_scene();

	while (SDL_PollEvent(event)
		|| status_ == JE_STATE_NONE) // state updating loop
	{
		InputHandler::update(*event);
		GLManager::update(*event);

		elapsedTime = timer_.get_elapsed_time(); // get elapsed time
		frameTime_ = elapsedTime - currentTime; // get frame time

		// Manually block the rfame skipping
		if (frameTime_ > MAX_FRAME_TIME)
			frameTime_ = MAX_FRAME_TIME;

		timeStack += frameTime_; // stack timer
		frames_++; // stack frames

		// Update the scene and systems
		if (timeStack >= SECOND) {

			currentTime = elapsedTime; // refresh the current time
			currentScene_->update(frameTime_); // update the current scene
			InputHandler::mouse_refresh(*event); // refresh mouse wheel status

			frames_ = 0;
			timeStack = 0.f;

			SDL_GL_SwapWindow(window_);
		}
	}

	switch (status_) {

		// Pause process
	case JE_STATE_PAUSE:
		/*SYSTEM::Pause();*/
		break;

		// The case to quit app
	case JE_STATE_QUIT:				
		//while (pCurrent_) {
		//	State* pLast = pCurrent_->pLastStage_;
		//	pCurrent_->Close();
		//	pCurrent_->Unload();
		//	pCurrent_ = pLast;
		//}
		break;

		// The case to resume to last state
	case JE_STATE_RESUME:				
		/*pCurrent_->Close();
		pCurrent_->Unload();
		SYSTEM::Resume();*/
		break;

		// The case to restart the current state
		// The case to change to next state
		// The case to resume and change
	case JE_STATE_RESTART:				
	case JE_STATE_CHANGE:				
	case JE_STATE_RESUME_AND_CHANGE:	
		//pCurrent_->Close();
		//pCurrent_->Unload();
		break;

		// Keep updaring - this should not happen
	case JE_STATE_NONE:
	default:
		break;
	}
}

void SceneManager::close()
{
	clear_scenes();
	Scene::unbind_system();
}

jeEnd