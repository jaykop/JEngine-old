#include "State.h"
#include "JsonParser.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "ObjectContainer.h"

#include "InputHandler.h"

JE_BEGIN

State::State(const char* _name)
	:m_pLastStage(nullptr)
{
	m_name.assign(_name);
}

void State::Load()
{
	JE_DEBUG_PRINT("Loading %s...\n", m_name.c_str());
	m_objContainer = new ObjectContainer; 
	JSON::ReadFile(m_loadDirectory.c_str());

	// TODO
	// Why not move to system
	JSON::LoadObjects(m_objContainer);
	SYSTEM::Load();
}

void State::Init()
{
	JE_DEBUG_PRINT("Initializing %s...\n", m_name.c_str());
	SYSTEM::Init();
}

void State::Update(float _dt)
{
	//JE_DEBUG_PRINT("Updating %s...\n", m_name.c_str());
	SYSTEM::Update(_dt);

	if (INPUT::KeyPressed(JE_ESC)) {
		JE_DEBUG_PRINT("Quit\n");
		STATE::Quit();
	}

}

void State::Close()
{
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
	SYSTEM::Close();
}

void State::Unload()
{
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
	SYSTEM::Unload();
	ClearObjectContainer();
}

void State::ClearObjectContainer()
{
	if (m_objContainer) {
		m_objContainer->ClearObjectMap();
		delete m_objContainer;
		m_objContainer = nullptr;
	}
}

JE_END

