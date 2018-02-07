#pragma once
#include "SDL.h"
#include "Macro.h"

// TODO
// Pyhton script later...
//#include "Python.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Engine class
//////////////////////////////////////////////////////////////////////////
class Engine {

public:

	static void OpenConsole();
	static void Ready();
	static void Run();
	static void CloseConsole();

	static bool m_IMGUI;

private:

	static bool m_functionOrder[4];

	Engine() = delete;
	~Engine() = delete;
	Engine(const Engine& /*_copy*/) = delete;
	void operator=(const Engine& /*_copy*/) = delete;
};

using JENGINE = Engine;

JE_END