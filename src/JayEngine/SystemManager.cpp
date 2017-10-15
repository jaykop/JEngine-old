#include "SystemManager.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "BehaviorSystem.h"

NS_JE_BEGIN

SoundSystem					*SystemManager::m_pSoundSystem = nullptr;
GraphicSystem				*SystemManager::m_pGraphicSystem = nullptr;
PhysicsSystem				*SystemManager::m_pPhysicsSystem = nullptr;
BehaviorSystem				*SystemManager::m_pBehaviorSystem = nullptr;
SystemManager::SystemStack	SystemManager::m_pauseStack;

void SystemManager::Load()
{
	m_pSoundSystem->Load();
	m_pGraphicSystem->Load();
	m_pPhysicsSystem->Load();
	m_pBehaviorSystem->Load();
}

void SystemManager::Init()
{
	m_pSoundSystem->Init();
	m_pGraphicSystem->Init();
	m_pPhysicsSystem->Init();
	m_pBehaviorSystem->Init();
}

void SystemManager::Update(float _dt)
{
	m_pGraphicSystem->Update(_dt);
	m_pPhysicsSystem->Update(_dt);
	m_pSoundSystem->Update(_dt);
	m_pBehaviorSystem->Update(_dt);
}

void SystemManager::Close()
{
	m_pSoundSystem->Close();
	m_pGraphicSystem->Close();
	m_pPhysicsSystem->Close();
	m_pBehaviorSystem->Close();
}

void SystemManager::Unload()
{
	m_pSoundSystem->Unload();
	m_pGraphicSystem->Unload();
	m_pPhysicsSystem->Unload();
	m_pBehaviorSystem->Unload();
}

void SystemManager::Pause()
{
	m_pauseStack.push(this);
}

void SystemManager::Resume()
{

}

void SystemManager::Bind()
{
	if (!m_binded) {
		m_pGraphicSystem = new GraphicSystem;
		m_pPhysicsSystem = new PhysicsSystem;
		m_pSoundSystem = new SoundSystem;
		m_pBehaviorSystem = new BehaviorSystem;
			
		m_binded = true;
	}
}

void SystemManager::Unbind()
{
	if (m_binded) {
		// Delete graphic system
		if (m_pGraphicSystem) {
			delete m_pGraphicSystem;
			m_pGraphicSystem = nullptr;
		}

		// Delete physic system
		if (m_pPhysicsSystem) {
			delete m_pPhysicsSystem;
			m_pPhysicsSystem = nullptr;
		}

		// Delete sound system
		if (m_pSoundSystem) {
			delete m_pSoundSystem;
			m_pSoundSystem = nullptr;
		}

		// Delete System
		if (m_pBehaviorSystem) {
			delete m_pBehaviorSystem;
			m_pBehaviorSystem = nullptr;
		}
	
		m_binded = false;
	}
}

NS_JE_END

