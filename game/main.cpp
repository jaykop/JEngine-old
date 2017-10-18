/******************************************************************************/
/*!
\file   Platform_win32.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2017/08/22(yy/mm/dd)

\description
Contains Process' main flow

*/
/******************************************************************************/

#include "Macro.h"
#include "Debug.h"
#include "Application.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

USING_NS_JE;

int main(int argc, char* args[]) {

	// Make console and check memory leak
	DEBUG_LEAK_CHECKS(-1);
	DEBUG_CREATE_CONSOLE();

	// Block the warnings
	JE_UNUSED_PARAM(argc);
	JE_UNUSED_PARAM(args);

	if (Application::Initialize())
		Application::Update();

	Application::Close();

	DEBUG_DESTROY_CONSOLE();

	return 0;

}