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

#include "Engine.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

USING_NS_JE;

int main(int argc, char* args[]) {

	JE_UNUSED_PARAM(argc);	// No argc - Block the warnings
	JE_UNUSED_PARAM(args);	// No args - Block the warnings

	JENGINE::m_IMGUI = false;	// Turn off imgui

	JENGINE::OpenConsole();
	JENGINE::Ready();		
	JENGINE::Run();			
	JENGINE::CloseConsole();

	return 0;

}