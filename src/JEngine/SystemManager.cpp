#include "SystemManager.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SYSTEM::SystemBlock	*SYSTEM::m_systems = nullptr;
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

void SystemManager::Load(CR_RJDoc _data)
{
	m_systems->Load(_data);
}

void SystemManager::Init()
{
	m_systems->Init();
}

void SystemManager::Update(const float _dt)
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
	m_systems = new SystemBlock;
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
		m_systems = new SystemBlock;
		m_systems->Bind();
	}
}

void SystemManager::Unbind()
{
	// Check valid ptr
	if (m_systems) {
		delete m_systems;
		m_systems = nullptr;
	}
}

jeEnd
