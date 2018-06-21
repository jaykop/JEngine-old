/******************************************************************************/
/*!
\file   main.cpp
\author Jeong Juyong
\par    email: jeykop14\@gmail.com
\date   2017/08/22(yy/mm/dd)

\description
Contains Process' main flow

*/
/******************************************************************************/

#include "SDL.h"
#include "JEngine.h"
#include "Macro.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

int main(int argc, char* args[]) {

	jeUnusedParam(argc);	// No argc - Block the warnings
	jeUnusedParam(args);	// No args - Block the warnings
	
	JEngine::imguiToggle = false;

	if (JEngine::RegisterAssets())
		return JEngine::Run();

	// Improper finish
	return -1;

}
