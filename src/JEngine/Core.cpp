#include "Core.h"
#include "Debug.h"
#include "Application.h"
#include "AssetManager.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Core class part
//////////////////////////////////////////////////////////////////////////

bool Core::m_IMGUI = false;

void Core::OpenConsole()
{
	// Pop console window 
	DEBUG_LEAK_CHECKS(-1);

	// and check memory leak
	DEBUG_CREATE_CONSOLE();
}

void Core::Run()
{
	// Open application
	if (APP::Initialize())
		APP::Update();

	APP::Close();
}

void Core::CloseConsole()
{
	// Delete console window
	DEBUG_DESTROY_CONSOLE();
}

JE_END