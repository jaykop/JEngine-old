#include "SystemManager.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "BehaviorSystem.h"

NS_JE_BEGIN

GraphicSystem	*SystemManager::m_grpSystem = nullptr;
PhysicsSystem	*SystemManager::m_phySystem = nullptr;
SoundSystem		*SystemManager::m_sndSystem = nullptr;
BehaviorSystem	*SystemManager::m_bhvSystem = nullptr;

void SystemManager::Load()
{
	m_grpSystem->Load();
	m_phySystem->Load();
	m_sndSystem->Load();
	m_bhvSystem->Load();
}

void SystemManager::Init()
{
	m_grpSystem->Init();
	m_phySystem->Init();
	m_sndSystem->Init();
	m_bhvSystem->Init();
}

void SystemManager::Update(float _dt)
{
	m_grpSystem->Update(_dt);
	m_phySystem->Update(_dt);
	m_sndSystem->Update(_dt);
	m_bhvSystem->Update(_dt);
}


void SystemManager::Close()
{
	m_grpSystem->Close();
	m_phySystem->Close();
	m_sndSystem->Close();
	m_bhvSystem->Close();
}

void SystemManager::Unload()
{
	m_grpSystem->Unload();
	m_phySystem->Unload();
	m_sndSystem->Unload();
	m_bhvSystem->Unload();
}

void SystemManager::Bind()
{
	m_grpSystem = new GraphicSystem{};
	m_phySystem = new PhysicsSystem;
	m_sndSystem = new SoundSystem;
	m_bhvSystem = new BehaviorSystem;
}

void SystemManager::Unbind()
{
	// Delete graphic system
	if (m_grpSystem) {
		delete m_grpSystem;
		m_grpSystem = nullptr;
	}

	// Delete physic system
	if (m_phySystem) {
		delete m_phySystem;
		m_phySystem = nullptr;
	}

	// Delete sound system
	if (m_sndSystem) {
		delete m_sndSystem;
		m_sndSystem = nullptr;
	}

	// Delete System
	if (m_bhvSystem) {
		delete m_bhvSystem;
		m_bhvSystem = nullptr;
	}
}



NS_JE_END

