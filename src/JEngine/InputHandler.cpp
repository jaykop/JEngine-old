#include "SDL.h"
#include "InputHandler.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
bool			INPUT::m_keyPressed = false;
bool			INPUT::m_mousePressed = false;
vec3			INPUT::m_rawPosition = vec3::ZERO;
vec3			INPUT::m_screenPosition = vec3::ZERO;
INPUT::KeyMap	INPUT::m_keys, INPUT::m_triggerList;
int				INPUT::m_mouseWheel = 0;
unsigned		INPUT::m_triggerCalled = 0;

void InputHandler::Init()
{
	// Refresh all keys' status
	for (unsigned i = 0; i < JE_KEY_END; ++i)
		m_triggerList[i] = m_keys[i] = false;
}

bool InputHandler::KeyPressed(JE_KEY _pressed)
{
	return m_keys[_pressed];
}

bool InputHandler::KeyTriggered(JE_KEY _trigger)
{
	if ((m_triggerCalled || !m_triggerList[_trigger])	// First press, trigger must be false or check trigger call is on the same frame
		&& m_keys[_trigger]) {							// Key pressed, so this must be true
		m_triggerList[_trigger] = true;					// Set trigger is activated -> no more access here
		m_triggerCalled++;								// Increase the number of calling trigger
		return true;
	}

	return false;
}

void InputHandler::KeyUp(void)
{
	m_keyPressed = false;
}

void InputHandler::KeyDown(void)
{
	m_keyPressed = true;
}

void InputHandler::MouseUp()
{
	m_mousePressed = false;
}

void InputHandler::MouseDown()
{
	m_mousePressed = true;
}

JE_KEY InputHandler::KeyTranslator(SDL_Event* _event)
{
	// Mouse translator
	switch (_event->button.button) {

	case SDL_BUTTON_RIGHT:
		return JE_MOUSE_RIGHT;
		break;

	case SDL_BUTTON_LEFT:
		return JE_MOUSE_LEFT;
		break;

	case SDL_BUTTON_MIDDLE:
		return JE_MOUSE_MIDDLE;
		break;
	}

	// Keyboard translator
	switch (_event->key.keysym.sym) {

	case SDLK_UNKNOWN:
		break;

	case SDLK_RETURN:
		return JE_ENTER;
		break;

	case SDLK_ESCAPE:
		return JE_ESC;
		break;

	case SDLK_BACKSPACE:
		return JE_BACK;
		break;

	case SDLK_TAB:
		return JE_TAB;
		break;

	case SDLK_SPACE:
		return JE_SPACE;
		break;

	case SDLK_RIGHT:
		return JE_RIGHT;
		break;

	case SDLK_LEFT:
		return JE_LEFT;
		break;

	case SDLK_DOWN:
		return JE_DOWN;
		break;

	case SDLK_UP:
		return JE_UP;
		break;

	case SDLK_0:
		return JE_0;
		break;

	case SDLK_1:
		return JE_1;
		break;

	case SDLK_2:
		return JE_2;
		break;

	case SDLK_3:
		return JE_3;
		break;

	case SDLK_4:
		return JE_4;
		break;

	case SDLK_5:
		return JE_5;
		break;

	case SDLK_6:
		return JE_6;
		break;

	case SDLK_7:
		return JE_7;
		break;

	case SDLK_8:
		return JE_8;
		break;

	case SDLK_9:
		return JE_9;
		break;

	case SDLK_a:
		return JE_A;
		break;

	case SDLK_b:
		return JE_B;
		break;

	case SDLK_c:
		return JE_C;
		break;

	case SDLK_d:
		return JE_D;
		break;

	case SDLK_e:
		return JE_E;
		break;

	case SDLK_f:
		return JE_F;
		break;

	case SDLK_g:
		return JE_G;
		break;

	case SDLK_h:
		return JE_H;
		break;

	case SDLK_i:
		return JE_I;
		break;

	case SDLK_j:
		return JE_J;
		break;

	case SDLK_k:
		return JE_K;
		break;

	case SDLK_l:
		return JE_L;
		break;

	case SDLK_m:
		return JE_M;
		break;

	case SDLK_n:
		return JE_N;
		break;

	case SDLK_o:
		return JE_O;
		break;

	case SDLK_p:
		return JE_P;
		break;

	case SDLK_q:
		return JE_Q;
		break;

	case SDLK_r:
		return JE_R;
		break;

	case SDLK_s:
		return JE_S;
		break;

	case SDLK_t:
		return JE_T;
		break;

	case SDLK_u:
		return JE_U;
		break;

	case SDLK_v:
		return JE_V;
		break;

	case SDLK_w:
		return JE_W;
		break;

	case SDLK_x:
		return JE_X;
		break;

	case SDLK_y:
		return JE_Y;
		break;

	case SDLK_z:
		return JE_Z;
		break;

	case SDLK_CAPSLOCK:
		return JE_CAPSLOCK;
		break;
		
	}

	return JE_NONE;
}

void InputHandler::Update(SDL_Event* _event)
{	
	// Initialize the number of calling trigger
	m_triggerCalled = 0;

	// Refresh the mouse wheel toggles
	switch (m_mouseWheel) {
	case 0:
		m_triggerList[JE_MOUSE_WHEEL_DOWN]
			= m_keys[JE_MOUSE_WHEEL_DOWN]
			= m_triggerList[JE_MOUSE_WHEEL_UP]
			= m_keys[JE_MOUSE_WHEEL_UP] = false;
		break;
	case 1:
		m_triggerList[JE_MOUSE_WHEEL_UP]
			= m_keys[JE_MOUSE_WHEEL_UP] = true;
		break;
	case -1:
		m_triggerList[JE_MOUSE_WHEEL_DOWN]
			= m_keys[JE_MOUSE_WHEEL_DOWN] = true;
		break;
	}

	// Handle input events
	switch (_event->type)
	{
		// Keyboard
	case SDL_KEYDOWN:
		m_keys[KeyTranslator(_event)] = true;
		break;

	case SDL_KEYUP:
		m_triggerList[KeyTranslator(_event)]
			= m_keys[KeyTranslator(_event)] = false;
		break;

		// Mouse
	case SDL_MOUSEBUTTONDOWN:
		MouseDown();
		m_keys[KeyTranslator(_event)] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		MouseUp();
		m_triggerList[KeyTranslator(_event)]
			= m_keys[KeyTranslator(_event)] = false;
		break;

	case SDL_MOUSEMOTION:
		m_rawPosition
			= vec3(float(_event->motion.x), float(_event->motion.y));
		break;

	case SDL_MOUSEWHEEL:
	{
		if (_event->wheel.y > 0)
			m_mouseWheel = 1;

		else if (_event->wheel.y < 0)
			m_mouseWheel = -1;

		_event->wheel.y = 0;
		break;
	}

	case SDL_WINDOWEVENT:
	case SDL_TEXTEDITING:
	default:
		break;
	}
}

vec3& InputHandler::GetRawPosition()
{
	return m_rawPosition;
}

vec3& InputHandler::GetOrhtoPosition()
{
	return m_screenPosition;
}

jeEnd
