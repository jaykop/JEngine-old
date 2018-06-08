#include "State.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "ObjectContainer.h"

jeBegin

State::State(const char* _name)
	:m_pLastStage(nullptr)
{
	m_name.assign(_name);
}

void State::Load()
{
	jeDebugPrint("*State - Loading %s...\n", m_name.c_str());
	
	// Allocate new object container;
	OBJECT::m_pSharedContainer 
		= m_pObjContainer = new ObjectContainer;
	
	// Read flie from json state file
	JSON::ReadFile(m_loadDirectory.c_str());

	// Load objects and components, and system information
	JSON::LoadObjects();
	SYSTEM::Load(JSON::GetDocument());
}

void State::Init()
{
#if defined(_DEBUG)
		STATE::m_showUpdateMessage = true;
#endif // _DEBUG

	jeDebugPrint("*State - Initializing %s...\n", m_name.c_str());
	SYSTEM::Init();
}

void State::Update(const float _dt)
{
#if defined(_DEBUG)
	if (STATE::m_showUpdateMessage) {
		jeDebugPrint("*State - Updating %s...\n", m_name.c_str());
		STATE::m_showUpdateMessage = false;
	}
#endif // _DEBUG

	SYSTEM::Update(_dt);
}

void State::Close()
{
#if defined(_DEBUG)
	STATE::m_showUpdateMessage = true;
#endif // _DEBUG

	OBJECT::m_pSharedContainer = STATE::GetCurrentState()->m_pObjContainer;

	jeDebugPrint("*State - Closing %s...\n", m_name.c_str());
	SYSTEM::Close();
}

void State::Unload()
{
	jeDebugPrint("*State - Unloading %s...\n", m_name.c_str());
	SYSTEM::Unload();
	ClearObjectContainer();
}

void State::ClearObjectContainer()
{
	if (m_pObjContainer) {
		m_pObjContainer->ClearObjectMap();
		delete m_pObjContainer;
		m_pObjContainer = nullptr;
	}
}

jeEnd

