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

#include "Core.h"

#pragma comment(lib, "sdl2")
#pragma comment(lib, "sdl2main")

USING_NS_JE;

int main(int argc, char* args[]) {

	JE_UNUSED_PARAM(argc);	// No argc - Block the warnings
	JE_UNUSED_PARAM(args);	// No args - Block the warnings

	CORE::Open(true);		// Set imgui toggle on

	// TODO
	//PyRun_SimpleString("test1 = 1\n"
	//	"test2 = 2\n"
	//	"test3 = test1 + test2\n");
	//PyObject *pModule = PyImport_AddModule("__main__"); //create main module
	//PyObject *catcher = PyObject_GetAttrString(pModule, "test3");
	//char *a = PyBytes_AsString(catcher);
	//printf("Here's the output: %s\n"); //it's not in our C++ portion

	CORE::Ready();			// Load user components and directory
	CORE::Run();			// Run application
	CORE::Close();			// Wrap up the app and close
	
	return 0;

}