#include "SystemManager.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "BehaviorSystem.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// class SystemManager::Systems's funciton bodues
//////////////////////////////////////////////////////////////////////////
SystemManager::SystemBlock::SystemBlock()
	:m_pGraphicSystem(nullptr), m_pSoundSystem(nullptr),
	m_pPhysicsSystem(nullptr), m_pBehaviorSystem(nullptr)
{}

SystemManager::SystemBlock::~SystemBlock()
{
	Unbind();
}

void SystemManager::SystemBlock::Bind()
{
	m_pGraphicSystem = new GraphicSystem;
	m_pPhysicsSystem = new PhysicsSystem;
	m_pSoundSystem = new SoundSystem;
	m_pBehaviorSystem = new BehaviorSystem;
}

void SystemManager::SystemBlock::Unbind()
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

void SystemManager::SystemBlock::Load(CR_RJDoc _data)
{
	m_pBehaviorSystem->Load(_data);
	m_pSoundSystem->Load(_data);
	m_pGraphicSystem->Load(_data);
	m_pPhysicsSystem->Load(_data);
}

void SystemManager::SystemBlock::Init()
{
	m_pSoundSystem->Init();
	m_pGraphicSystem->Init();
	m_pPhysicsSystem->Init();
	m_pBehaviorSystem->Init();
}

void SystemManager::SystemBlock::Update(float dt)
{
	m_pGraphicSystem->Update(dt);
	m_pPhysicsSystem->Update(dt);
	m_pSoundSystem->Update(dt);
	m_pBehaviorSystem->Update(dt);
}

void SystemManager::SystemBlock::Close()
{
	m_pSoundSystem->Close();
	m_pGraphicSystem->Close();
	m_pPhysicsSystem->Close();
	m_pBehaviorSystem->Close();
}

void SystemManager::SystemBlock::Unload()
{
	m_pSoundSystem->Unload();
	m_pGraphicSystem->Unload();
	m_pPhysicsSystem->Unload();
	m_pBehaviorSystem->Unload();
}

jeEnd
