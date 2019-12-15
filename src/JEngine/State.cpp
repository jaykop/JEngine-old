#include "State.h"
#include "StateManager.h"
#include "SystemManager.h"
#include "ObjectContainer.h"

jeBegin

State::State(const char* name)
	:pLastStage_(nullptr)
{
	name_.assign(name);
}

void State::Load()
{
	jeDebugPrint("*State - Loading %s...\n", name_.c_str());
	
	// Allocate new object container;
	OBJECT::pContainer_
		= pObjContainer_ = new ObjectContainer;
	
	// Read flie from json state file
	JSON::ReadFile(loadDirectory_.c_str());

	// Load objects and components, and system information
	JSON::LoadObjects();
	SYSTEM::Load(JSON::GetDocument());
}

void State::Init()
{
#if defined(_DEBUG)
		STATE::showUpdateMessage_ = true;
#endif // _DEBUG

	jeDebugPrint("*State - Initializing %s...\n", name_.c_str());
	SYSTEM::Init();
}

void State::Update(float dt)
{
#if defined(_DEBUG)
	if (STATE::showUpdateMessage_) {
		jeDebugPrint("*State - Updating %s...\n", name_.c_str());
		STATE::showUpdateMessage_ = false;
	}
#endif // _DEBUG

	SYSTEM::Update(dt);
}

void State::Close()
{
#if defined(_DEBUG)
	STATE::showUpdateMessage_ = true;
#endif // _DEBUG

	OBJECT::pContainer_ = STATE::GetCurrentState()->pObjContainer_;

	jeDebugPrint("*State - Closing %s...\n", name_.c_str());
	SYSTEM::Close();
}

void State::Unload()
{
	jeDebugPrint("*State - Unloading %s...\n", name_.c_str());
	SYSTEM::Unload();
	ClearObjectContainer();
}

void State::ClearObjectContainer()
{
	if (pObjContainer_) {
		pObjContainer_->ClearObjectMap();
		delete pObjContainer_;
		pObjContainer_ = nullptr;
	}
}

const char* State::GetName() const
{
	return name_.c_str();
}


jeEnd

