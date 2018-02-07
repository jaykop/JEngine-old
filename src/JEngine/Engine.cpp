#include "Engine.h"
#include "Debug.h"
#include "Application.h"
#include "AssetManager.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// Engine class part
//////////////////////////////////////////////////////////////////////////

bool Engine::m_functionOrder[4] = { false, false, false, false };
bool Engine::m_IMGUI = false;

void Engine::OpenConsole()
{
	if (m_functionOrder[1] || m_functionOrder[2] || m_functionOrder[3])
		JE_DEBUG_PRINT("!Engine - OpenConsole function must be called firstly.\n");

	else if (m_functionOrder[0])
		JE_DEBUG_PRINT("!Engine - You called OpenConsole function twice.\n");

	else {
		// Pop console window 
		DEBUG_LEAK_CHECKS(-1);

		// and check memory leak
		DEBUG_CREATE_CONSOLE();

		m_functionOrder[0] = true;
	}
}

void Engine::Ready()
{
	if (m_functionOrder[2] || !m_functionOrder[0] || m_functionOrder[3])
		JE_DEBUG_PRINT("!Engine - Ready function must be called secondly.\n");

	else if (m_functionOrder[1])
		JE_DEBUG_PRINT("!Engine - You called Ready function twice.\n");

	else {
		// Get assets ready 
		ASSET::RegisterAssets();

		m_functionOrder[1] = true;
	}
}

void Engine::Run()
{
	if (!m_functionOrder[1] || !m_functionOrder[0] || m_functionOrder[3])
		JE_DEBUG_PRINT("!Engine - Run function must be called thirdly.\n");

	else if (m_functionOrder[2])
		JE_DEBUG_PRINT("!Engine - You called Run function twice.\n");

	else {
		// Open application
		if (APP::Initialize())
			APP::Update();

		APP::Close();

		m_functionOrder[2] = true;
	}
}

void Engine::CloseConsole()
{
	if (!m_functionOrder[0] || !m_functionOrder[1] || !m_functionOrder[2])
		JE_DEBUG_PRINT("!Engine - CloseConsole function must be called lastly.\n");
	
	else if (m_functionOrder[3])
		JE_DEBUG_PRINT("!Engine - You called CloseConsole function twice.\n");

	else {
		// Delete console window
		DEBUG_DESTROY_CONSOLE();
	}
}

JE_END