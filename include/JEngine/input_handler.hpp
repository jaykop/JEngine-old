/******************************************************************************/
/*!
\file   input_handler.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/11(yy/mm/dd)

\description
Contains the definition of InputHandler class
*/
/******************************************************************************/
#pragma once

#include <SDL.h>
#include <key_map.hpp>
#include <unordered_map>

jeBegin

using KeyMap = std::unordered_map<KEY, bool>;

class InputHandler {

	friend class Application;
	friend class SceneManager;

public:

	enum MouseWheel { NONE, UP, DOWN } ;

	static bool any_key_down(); // check both mouse or key
	static bool any_mouse_down();
	static bool any_input_down();

	static bool key_pressed(KEY key);
	static bool key_triggered(KEY key);

	static InputHandler::MouseWheel get_mouse_wheel_status();
	static float wheelSensitivity_;

	// mouse position
	
private:

	static KEY key_translator(const SDL_Event& event);
	static KEY mouse_translator(const SDL_Event& event);
	static void mouse_refresh(SDL_Event& event);

	static void initialize();
	static void update(const SDL_Event& event);
	static void close();

	static bool mouseDown, keyDown;
	static KeyMap keyMap, triggerMap;
	static MouseWheel mouseWheel_;

	InputHandler() = delete;
	~InputHandler() = delete;
	InputHandler(InputHandler&&) = delete;
	InputHandler& operator=(InputHandler&&) = delete;
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

};

jeEnd