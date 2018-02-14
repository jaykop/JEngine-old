#pragma once
#include <map>
#include "Vector3.h"

union SDL_Event;

JE_BEGIN

enum JE_KEY  {

	// Locked useless keys for a sec
	/*	JE_EXCLAIM = SDLK_EXCLAIM ,
	JE_QUOTEDBL = SDLK_QUOTEDBL ,
	JE_HASH = SDLK_HASH ,
	JE_PERCENT = SDLK_PERCENT ,
	JE_DOLLAR = SDLK_DOLLAR ,
	JE_AMPERSAND = SDLK_AMPERSAND ,
	JE_QUOTE = SDLK_QUOTE ,
	JE_LEFTPAREN = SDLK_LEFTPAREN ,
	JE_RIGHTPAREN = SDLK_RIGHTPAREN ,
	JE_ASTERISK = SDLK_ASTERISK ,
	JE_PLUS = SDLK_PLUS ,
	JE_COMMA = SDLK_COMMA ,
	JE_MINUS = SDLK_MINUS ,
	JE_PERIOD = SDLK_PERIOD ,
	JE_SLASH = SDLK_SLASH,
	JE_COLON = SDLK_COLON,
	JE_SEMICOLON = SDLK_SEMICOLON,
	JE_LESS = SDLK_LESS,
	JE_EQUALS = SDLK_EQUALS ,
	JE_GREATER = SDLK_GREATER ,
	JE_QUESTION = SDLK_QUESTION ,
	JE_AT = SDLK_AT,
	JE_LEFTBRACKET = SDLK_LEFTBRACKET,
	JE_BACKSLASH = SDLK_BACKSLASH,
	JE_RIGHTBRACKET = SDLK_RIGHTBRACKET,
	JE_CARET = SDLK_CARET,
	JE_UNDERSCORE = SDLK_UNDERSCORE,
	JE_BACKQUOTE = SDLK_BACKQUOTE,
	JE_PRINTSCREEN = SDLK_PRINTSCREEN,
	JE_SCROLLLOCK = SDLK_SCROLLLOCK,
	JE_PAUSE = SDLK_PAUSE,
	JE_INSERT = SDLK_INSERT,
	JE_HOME = SDLK_HOME,
	JE_PAGEUP = SDLK_PAGEUP,
	JE_DELETE = SDLK_DELETE,
	JE_END = SDLK_END,
	JE_PAGEDOWN = SDLK_PAGEDOWN,*/
	//JE_F1 = SDLK_F1,
	//JE_F2 = SDLK_F2,
	//JE_F3 = SDLK_F3,
	//JE_F4 = SDLK_F4,
	//JE_F5 = SDLK_F5,
	//JE_F6 = SDLK_F6,
	//JE_F7 = SDLK_F7,
	//JE_F8 = SDLK_F8,
	//JE_F9 = SDLK_F9,
	//JE_F10 = SDLK_F10,
	//JE_F11 = SDLK_F11,
	//JE_F12 = SDLK_F12,

	// Keyboard
	// Normal funcs
	JE_NONE, JE_ENTER, JE_ESC, JE_BACK, JE_TAB, JE_SPACE,
	JE_RIGHT, JE_LEFT, JE_DOWN, JE_UP, JE_CAPSLOCK,

	// Numbers
	JE_0, JE_1, JE_2, JE_3, JE_4, JE_5, JE_6, JE_7, JE_8, JE_9,

	// Alphabet
	JE_A, JE_B, JE_C, JE_D,	JE_E, JE_F,	JE_G, JE_H, JE_I, JE_J, JE_K,
	JE_L, JE_M, JE_N, JE_O, JE_P, JE_Q, JE_R, JE_S, JE_T, JE_U, JE_V,
	JE_W, JE_X, JE_Y, JE_Z,

	// Mouse 
	JE_MOUSE_LEFT, JE_MOUSE_RIGHT, JE_MOUSE_MIDDLE,
	JE_MOUSE_WHEEL_UP, JE_MOUSE_WHEEL_DOWN,

	JE_KEY_END
};

class InputHandler {

	friend class StateManager;
	friend class GraphicSystem;
	using KeyMap = std::map<unsigned, bool> ;

public:
	
	// Basic key triggers
	static bool KeyPressed(JE_KEY _pressed);
	static bool KeyTriggered(JE_KEY _trigger);

	static vec3& GetRawPosition();
	static vec3& GetOrhtoPosition();

private:

	InputHandler();
	~InputHandler() = delete;
	InputHandler(const InputHandler& _handle) = delete;
	void operator=(const InputHandler& _handle) = delete;

	// Keyboard
	static void KeyUp();
	static void KeyDown();

	// Mouse
	static void MouseUp();
	static void MouseDown();

	static void Update(SDL_Event* _event);

	static JE_KEY	KeyTranslator(SDL_Event* _event);

	static vec3		m_screenPosition, m_rawPosition;
	static bool		m_keyPressed, m_mousePressed, m_wheelMoved;
	static KeyMap	m_keys, m_triggerList;
};

using INPUT = InputHandler;

JE_END
