#include "CustomComponent.h"
#include "BehaviorSystem.h"
#include "MessageDispatcher.h"
#include "Object.h"

jeBegin

BehaviorSystem::BehaviorSystem()
	:System()
{}

void BehaviorSystem::AddBehavior(CustomComponent* _behavior)
{
	m_behaviors.push_back(_behavior);
}

void BehaviorSystem::RemoveBehavior(CustomComponent* _behavior)
{
	for (Behaviors::iterator it = m_behaviors.begin();
		it != m_behaviors.end(); ++it) {
		if ((*it)->m_pOwner->GetId() == _behavior->m_pOwner->GetId()) {
			m_behaviors.erase(it);
			break;
		}
	}
}

void BehaviorSystem::Load(CR_RJDoc /*_data*/)
{
	// None...
}

void BehaviorSystem::Init()
{
	for (auto behavior : m_behaviors)
		behavior->Init();
}

void BehaviorSystem::Update(const float _dt)
{
	// Update normal game logics
	for (auto behavior : m_behaviors) 
		behavior->Update(_dt);

	// Deal with delayed event
	DISPATCHER::DispatchDelayedMessage();
}

void BehaviorSystem::Close()
{
	for (auto behavior : m_behaviors) 
		behavior->Close();
}

void BehaviorSystem::Unload()
{
	// TODO
	// Is this really necessary?
	for (auto behavior : m_behaviors) {
		behavior->Unload();
	}

	m_behaviors.clear();
}

jeEnd
