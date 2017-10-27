#include "SystemManager.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "BehaviorSystem.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SYSTEM::Systems		*SYSTEM::m_systems = nullptr;
SYSTEM::SystemStack	SYSTEM::m_pauseStack;

//////////////////////////////////////////////////////////////////////////
// class SystemManager's funciton bodues
//////////////////////////////////////////////////////////////////////////
SoundSystem* SystemManager::GetSoundSystem()
{
	return m_systems->m_pSoundSystem;
}

GraphicSystem* SystemManager::GetGraphicSystem()
{
	return m_systems->m_pGraphicSystem;;
}

PhysicsSystem* SystemManager::GetPhysicsSystem()
{
	return m_systems->m_pPhysicsSystem;
}

BehaviorSystem* SystemManager::GetBehaviorSystem()
{
	return m_systems->m_pBehaviorSystem;
}

void SystemManager::Load()
{
	m_systems->Load();
}

void SystemManager::Init()
{
	m_systems->Init();
}

void SystemManager::Update(float _dt)
{
	m_systems->Update(_dt);
}

void SystemManager::Close()
{
	m_systems->Close();
}

void SystemManager::Unload()
{
	m_systems->Unload();
}

void SystemManager::Pause()
{
	// Push current systems into the storage stack
	m_pauseStack.push(m_systems);

	// Bind new system
	m_systems = new Systems;
	m_systems->Bind();
}

void SystemManager::Resume()
{
	// Unbind systems
	m_systems->Unbind();

	// Delete and get last system
	delete m_systems;
	m_systems = nullptr;
	m_systems = m_pauseStack.top();

	// Pop the top(currnet system)
	m_pauseStack.pop();
}

void SystemManager::Bind()
{
	// Check nullptr
	if (!m_systems) {
		m_systems = new Systems;
		m_systems->Bind();
	}
}

void SystemManager::Unbind()
{
	// Check valid ptr
	if (m_systems) {
		m_systems->Unbind();
		delete m_systems;
		m_systems = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
// class SystemManager::Systems's funciton bodues
//////////////////////////////////////////////////////////////////////////
SystemManager::Systems::Systems()
	:m_pGraphicSystem(nullptr), m_pSoundSystem(nullptr),
	m_pPhysicsSystem(nullptr), m_pBehaviorSystem(nullptr)
{}

void SystemManager::Systems::Bind()
{
	m_pGraphicSystem = new GraphicSystem;
	m_pPhysicsSystem = new PhysicsSystem;
	m_pSoundSystem = new SoundSystem;
	m_pBehaviorSystem = new BehaviorSystem;
}

void SystemManager::Systems::Unbind()
{
	// Delete all systems
	if (m_pGraphicSystem) {
		delete m_pGraphicSystem;
		m_pGraphicSystem = nullptr;
	}

	if (m_pPhysicsSystem) {
		delete m_pPhysicsSystem;
		m_pPhysicsSystem = nullptr;
	}

	if (m_pSoundSystem) {
		delete m_pSoundSystem;
		m_pSoundSystem = nullptr;
	}

	if (m_pBehaviorSystem) {
		delete m_pBehaviorSystem;
		m_pBehaviorSystem = nullptr;
	}
}

void SystemManager::Systems::Load()
{
	m_pSoundSystem->Load();
	m_pGraphicSystem->Load();
	m_pPhysicsSystem->Load();
	m_pBehaviorSystem->Load();
}

void SystemManager::Systems::Init()
{
	m_pSoundSystem->Init();
	m_pGraphicSystem->Init();
	m_pPhysicsSystem->Init();
	m_pBehaviorSystem->Init();
}

void SystemManager::Systems::Update(float _dt)
{
	m_pGraphicSystem->Update(_dt);
	m_pPhysicsSystem->Update(_dt);
	m_pSoundSystem->Update(_dt);
	m_pBehaviorSystem->Update(_dt);
}

void SystemManager::Systems::Close()
{
	m_pSoundSystem->Close();
	m_pGraphicSystem->Close();
	m_pPhysicsSystem->Close();
	m_pBehaviorSystem->Close();
}

void SystemManager::Systems::Unload()
{
	m_pSoundSystem->Unload();
	m_pGraphicSystem->Unload();
	m_pPhysicsSystem->Unload();
	m_pBehaviorSystem->Unload();
}

JE_END
