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

#include "Debug.h"
#include "Application.h"
#include "AssetManager.h"

// TODO
// Pyhton script later...
//#include "Python.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

USING_NS_JE;

int main(int argc, char* args[]) {

	DEBUG_LEAK_CHECKS(-1);	// Pop console window 
	DEBUG_CREATE_CONSOLE(); // and check memory leak

	JE_UNUSED_PARAM(argc);	// No argc - Block the warnings
	JE_UNUSED_PARAM(args);	// No args - Block the warnings

	ASSET::RegisterAssets(); // Load assets
	JENGINE::Run();			 // Run Engine
	
	DEBUG_DESTROY_CONSOLE(); // Delete console window

	return 0;

}