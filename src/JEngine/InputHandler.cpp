#include "SDL.h"
#include "InputHandler.h"
#include "GLManager.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
bool			INPUT::keyPressed_ = false;
bool			INPUT::mousePressed_ = false;
vec3			INPUT::rawPosition_ = vec3::ZERO;
vec3			INPUT::screenPosition_ = vec3::ZERO;
INPUT::KeyMap	INPUT::keys_, INPUT::triggerList_;
int				INPUT::mouseWheel_ = 0;
unsigned		INPUT::triggerCalled_ = 0;

void InputHandler::Init()
{
	// Refresh all keys' status_
	for (unsigned i = 0; i < JE_KEY_END; ++i)
		triggerList_[i] = keys_[i] = false;
}

bool InputHandler::KeyPressed(JE_KEY pressed)
{
	return keys_[pressed];
}

bool InputHandler::KeyTriggered(JE_KEY trigger)
{
	if ((triggerCalled_ || !triggerList_[trigger])	// First press, trigger must be false or check trigger call is on the same frame
		&& keys_[trigger]) {							// Key pressed, so this must be true
		triggerList_[trigger] = true;					// Set trigger is activated -> no more access here
		triggerCalled_++;								// Increase the number of calling trigger
		return true;
	}

	return false;
}

JE_KEY InputHandler::KeyTranslator(SDL_Event* event)
{
	// Mouse translator
	switch (event->button.button) {

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
	switch (event->key.keysym.sym) {

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

void InputHandler::Update(SDL_Event* event)
{	
	// Initialize the number of calling trigger
	triggerCalled_ = 0;

	// Refresh the mouse wheel toggles
	switch (mouseWheel_) {
	case 0:
		triggerList_[JE_MOUSE_WHEEL_DOWN]
			= keys_[JE_MOUSE_WHEEL_DOWN]
			= triggerList_[JE_MOUSE_WHEEL_UP]
			= keys_[JE_MOUSE_WHEEL_UP] = false;
		break;
	case 1:
		triggerList_[JE_MOUSE_WHEEL_UP]
			= keys_[JE_MOUSE_WHEEL_UP] = true;
		break;
	case -1:
		triggerList_[JE_MOUSE_WHEEL_DOWN]
			= keys_[JE_MOUSE_WHEEL_DOWN] = true;
		break;
	}

	// Handle input events
	switch (event->type)
	{
		// Keyboard
	case SDL_KEYDOWN:
		keyPressed_ = true;
		keys_[KeyTranslator(event)] = true;
		break;

	case SDL_KEYUP:
		keyPressed_ = false;
		triggerList_[KeyTranslator(event)]
			= keys_[KeyTranslator(event)] = false;
		break;

		// Mouse
	case SDL_MOUSEBUTTONDOWN:
		mousePressed_ = true;
		keys_[KeyTranslator(event)] = true;
		break;

	case SDL_MOUSEBUTTONUP:
		mousePressed_ = false;
		triggerList_[KeyTranslator(event)]
			= keys_[KeyTranslator(event)] = false;
		break;

	case SDL_MOUSEMOTION:
		rawPosition_
			= vec3(float(event->motion.x), float(event->motion.y));
		break;

	case SDL_MOUSEWHEEL:
	{
		if (event->wheel.y > 0)
			mouseWheel_ = 1;

		else if (event->wheel.y < 0)
			mouseWheel_ = -1;

		event->wheel.y = 0;
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
	return rawPosition_;
}

vec3& InputHandler::GetOrhtoPosition()
{
	float width = GLM::width_* .5f, height = GLM::height_* .5f;
	screenPosition_.Set(rawPosition_.x - width, height - rawPosition_.y, 0./*m_mouseZ*/);
	return screenPosition_;
}

bool InputHandler::AnyKeyDown()
{
	return keyPressed_ || mousePressed_;
}

bool InputHandler::KeyDown()
{
	return keyPressed_;
}

bool InputHandler::MouseDown()
{
	return mousePressed_;
}

jeEnd
