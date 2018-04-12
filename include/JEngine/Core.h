#pragma once
#include "SDL.h"
#include "Macro.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Core class
//////////////////////////////////////////////////////////////////////////
class Core {

	friend class ImguiManager;

public:

	static void Open(bool _imgui);
	static void Ready();
	static void Run();
	static void Close();
	
private:

	static bool m_IMGUI;

	Core() = delete;
	~Core() = delete;
	Core(const Core& /*_copy*/) = delete;
	void operator=(const Core& /*_copy*/) = delete;
};

using CORE = Core;

JE_END