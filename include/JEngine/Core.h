#pragma once
#include "SDL.h"
#include "Macro.h"

// TODO
// Pyhton script later...
//#include "Python.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Core class
//////////////////////////////////////////////////////////////////////////
class Core {

public:

	static void OpenConsole();
	static void Ready();
	static void Run();
	static void CloseConsole();

	static bool m_IMGUI;

private:
	
	Core() = delete;
	~Core() = delete;
	Core(const Core& /*_copy*/) = delete;
	void operator=(const Core& /*_copy*/) = delete;
};

using CORE = Core;

JE_END