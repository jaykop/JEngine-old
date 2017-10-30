#include "State.h"
#include "JsonParser.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "ObjectContainer.h"

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
	//JSON::LoadObjects();
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
}

void State::Close()
{
	JE_DEBUG_PRINT("Closing %s...\n", m_name.c_str());
	SYSTEM::Close();
}

void State::Unload()
{
	JE_DEBUG_PRINT("Unloading %s...\n", m_name.c_str());
	ClearObjectContainer();
	SYSTEM::Unload();
}

void State::ClearObjectContainer()
{
	if (m_objContainer) {
		delete m_objContainer;
		m_objContainer = nullptr;
	}
}

JE_END

