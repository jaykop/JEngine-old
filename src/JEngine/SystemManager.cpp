#include "SystemManager.h"
#include "GraphicSystem.h"
#include "PhysicsSystem.h"
#include "SoundSystem.h"
#include "BehaviorSystem.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
SYSTEM::SystemBlock	*SYSTEM::pBlock_ = nullptr;
SYSTEM::SystemStack	SYSTEM::pauseStack_;

SoundSystem*		SYSTEM::pSound_ = nullptr;
GraphicSystem*		SYSTEM::pGraphic_ = nullptr;
PhysicsSystem*		SYSTEM::pPhysics_ = nullptr;
BehaviorSystem*		SYSTEM::pBehavior_ = nullptr;

//////////////////////////////////////////////////////////////////////////
// class SystemManager's funciton bodues
//////////////////////////////////////////////////////////////////////////
void SystemManager::Load(CR_RJDoc data)
{
	pBehavior_->Load(data);
	pSound_->Load(data);
	pGraphic_->Load(data);
	pPhysics_->Load(data);
}

void SystemManager::Init()
{
	pSound_->Init();
	pGraphic_->Init();
	pPhysics_->Init();
	pBehavior_->Init();
}

void SystemManager::Update(float dt)
{
	pGraphic_->Update(dt);
	pPhysics_->Update(dt);
	pSound_->Update(dt);
	pBehavior_->Update(dt);
}

void SystemManager::Close()
{
	pSound_->Close();
	pGraphic_->Close();
	pPhysics_->Close();
	pBehavior_->Close();
}

void SystemManager::Unload()
{
	pSound_->Unload();
	pGraphic_->Unload();
	pPhysics_->Unload();
	pBehavior_->Unload();
}

void SystemManager::Pause()
{
	// Push current systems into the storage stack
	pauseStack_.push(pBlock_);

	// Bind new system
	pBlock_ = nullptr;
	Bind();
}

void SystemManager::Resume()
{
	// Unbind systems
	Unbind();
	pBlock_ = pauseStack_.top();

	// Pop the top(currnet system)
	pauseStack_.pop();
}

void SystemManager::Bind()
{
	// Check nullptr
	if (!pBlock_) {

		pBlock_ = new SystemBlock;

		pBlock_->pGraphicSystem = pGraphic_ = new GraphicSystem;
		pBlock_->pPhysicsSystem = pPhysics_ = new PhysicsSystem;
		pBlock_->pSoundSystem = pSound_ = new SoundSystem;
		pBlock_->pBehaviorSystem = pBehavior_ = new BehaviorSystem;

	}
}

void SystemManager::Unbind()
{
	// Check valid ptr
	if (pBlock_) {

		// Delete all systems
		if (pGraphic_) {
			delete pGraphic_;
			pGraphic_ = nullptr;
		}

		if (pPhysics_) {
			delete pPhysics_;
			pPhysics_ = nullptr;
		}

		if (pSound_) {
			delete pSound_;
			pSound_ = nullptr;
		}

		if (pBehavior_) {
			delete pBehavior_;
			pBehavior_ = nullptr;
		}

		delete pBlock_;
		pBlock_ = nullptr;
	}
}

jeEnd
