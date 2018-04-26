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

	JE_UNUSED_PARAM(argc);	// No argc - Block the warnings
	JE_UNUSED_PARAM(args);	// No args - Block the warnings

	JEngine::RegisterAssets();
	JEngine::Run();

	return 0;

}
