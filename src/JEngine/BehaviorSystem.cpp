#include "CustomComponent.h"
#include "BehaviorSystem.h"
#include "MessageDispatcher.h"
#include "Object.h"

jeBegin

BehaviorSystem::BehaviorSystem()
	:System()
{}

void BehaviorSystem::AddBehavior(CustomComponent* behavior)
{
	behaviors_.push_back(behavior);
}

void BehaviorSystem::RemoveBehavior(CustomComponent* behavior)
{
	for (Behaviors::iterator it = behaviors_.begin();
		it != behaviors_.end(); ++it) {
		if ((*it)->GetOwner()->GetId() == behavior->GetOwner()->GetId()) {
			behaviors_.erase(it);
			break;
		}
	}
}

void BehaviorSystem::Load(CR_RJDoc /*data*/)
{
	// None...
}

void BehaviorSystem::Init()
{
	for (auto behavior : behaviors_)
		behavior->Init();
}

void BehaviorSystem::Update(float dt)
{
	// Update normal game logics
	for (auto behavior : behaviors_) 
		behavior->Update(dt);

	// Deal with delayed event
	DISPATCHER::DispatchDelayedMessage();
}

void BehaviorSystem::Close()
{
	for (auto behavior : behaviors_) 
		behavior->Close();
}

void BehaviorSystem::Unload()
{
	// TODO
	// Is this really necessary?
	for (auto behavior : behaviors_) {
		behavior->Unload();
	}

	behaviors_.clear();
}

jeEnd
