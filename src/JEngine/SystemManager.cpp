#include "SystemManager.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SYSTEM::SystemBlock	*SYSTEM::m_systemBlock = nullptr;
SYSTEM::SystemStack	SYSTEM::m_pauseStack;

//////////////////////////////////////////////////////////////////////////
// class SystemManager's funciton bodues
//////////////////////////////////////////////////////////////////////////
SoundSystem* SystemManager::GetSoundSystem()
{
	return m_systemBlock->m_pSoundSystem;
}

GraphicSystem* SystemManager::GetGraphicSystem()
{
	return m_systemBlock->m_pGraphicSystem;;
}

PhysicsSystem* SystemManager::GetPhysicsSystem()
{
	return m_systemBlock->m_pPhysicsSystem;
}

BehaviorSystem* SystemManager::GetBehaviorSystem()
{
	return m_systemBlock->m_pBehaviorSystem;
}

void SystemManager::Load(CR_RJDoc _data)
{
	m_systemBlock->Load(_data);
}

void SystemManager::Init()
{
	m_systemBlock->Init();
}

void SystemManager::Update(float dt)
{
	m_systemBlock->Update(dt);
}

void SystemManager::Close()
{
	m_systemBlock->Close();
}

void SystemManager::Unload()
{
	m_systemBlock->Unload();
}

void SystemManager::Pause()
{
	// Push current systems into the storage stack
	m_pauseStack.push(m_systemBlock);

	// Bind new system
	m_systemBlock = new SystemBlock;
	m_systemBlock->Bind();
}

void SystemManager::Resume()
{
	// Unbind systems
	m_systemBlock->Unbind();

	// Delete and get last system
	delete m_systemBlock;
	m_systemBlock = nullptr;
	m_systemBlock = m_pauseStack.top();

	// Pop the top(currnet system)
	m_pauseStack.pop();
}

void SystemManager::Bind()
{
	// Check nullptr
	if (!m_systemBlock) {
		m_systemBlock = new SystemBlock;
		m_systemBlock->Bind();
	}
}

void SystemManager::Unbind()
{
	// Check valid ptr
	if (m_systemBlock) {
		delete m_systemBlock;
		m_systemBlock = nullptr;
	}
}

jeEnd
