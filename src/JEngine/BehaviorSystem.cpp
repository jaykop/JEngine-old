#include "State.h"
#include "UserComponent.h"
#include "StateManager.h"
#include "BehaviorSystem.h"
#include "ObjectContainer.h"

JE_BEGIN

BehaviorSystem::BehaviorSystem()
	:System()
{}

void BehaviorSystem::AddBehavior(UserComponent* _behavior)
{
	m_behaviors.push_back(_behavior);
}

void BehaviorSystem::RemoveBehavior(UserComponent* _behavior)
{
	for (Behaviors::iterator it = m_behaviors.begin();
		it != m_behaviors.end(); ++it) {
		if ((*it)->m_pOwnerId == _behavior->m_pOwnerId) {
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
	for (auto behavior : m_behaviors){
		behavior->Init();
	}
}

void BehaviorSystem::Update(const float _dt)
{
	for (auto behavior : m_behaviors) {
		behavior->Update(_dt);
	}
}

void BehaviorSystem::Close()
{
	for (auto behavior : m_behaviors) {
		behavior->Close();
	}
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

JE_END