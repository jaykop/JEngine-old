#include "State.h"
#include "Component.h"
#include "StateManager.h"
#include "BehaviorSystem.h"
#include "ObjectContainer.h"

JE_BEGIN

BehaviorSystem::BehaviorSystem()
	:System()
{
	m_pObjectMap = 
		&STATE::GetCurrentState()->m_objContainer->GetObjectMap();
}

void BehaviorSystem::Load()
{
	// TODO
	//for (auto obj = m_pObjectMap->begin();
	//	obj != m_pObjectMap->end(); ++obj) {

	//	for (auto component = obj->second->GetComponentMap().begin();
	//		component != obj->second->GetComponentMap().end(); ++component){
	//		component->second->Load();
	//	}
	//}
}

void BehaviorSystem::Init()
{
	for (auto obj = m_pObjectMap->begin();
		obj != m_pObjectMap->end(); ++obj) {

		for (auto component = obj->second->GetComponentMap().begin();
			component != obj->second->GetComponentMap().end(); ++component) {
			component->second->Init();
		}
	}
}

void BehaviorSystem::Update(float _dt)
{
	for (auto obj = m_pObjectMap->begin();
		obj != m_pObjectMap->end(); ++obj) {

		for (auto component = obj->second->GetComponentMap().begin();
			component != obj->second->GetComponentMap().end(); ++component) {
			component->second->Update(_dt);
		}
	}
}

void BehaviorSystem::Close()
{
	for (auto obj = m_pObjectMap->begin();
		obj != m_pObjectMap->end(); ++obj) {

		for (auto component = obj->second->GetComponentMap().begin();
			component != obj->second->GetComponentMap().end(); ++component) {
			component->second->Close();
		}
	}
}

void BehaviorSystem::Unload()
{
	for (auto obj = m_pObjectMap->begin();
		obj != m_pObjectMap->end(); ++obj) {

		for (auto component = obj->second->GetComponentMap().begin();
			component != obj->second->GetComponentMap().end(); ++component) {
			component->second->Unload();
		}
	}
}

JE_END
