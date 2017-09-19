#include "Object.h"
#include "ObjectManager.h"
#include "ComponentManager.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
ObjectContainer ObjectManager::m_objContainer;
Object*			ObjectManager::m_pLastMade = nullptr;

void ObjectManager::CreateObject(const char * _name)
{
	// If there is created object in advance,
	// remove it
	if (m_pLastMade)
		RemoveObject(m_pLastMade->m_name.c_str());

	// Make new object
	m_pLastMade = new Object(_name);
}

Object* ObjectManager::GetCreatedObject()
{
	// Return the lastly created one
	return m_pLastMade;
}

void ObjectManager::AddCreatedObject()
{
	// Register the object
	m_objContainer.insert(
		ObjectContainer::value_type(
			m_pLastMade->m_name, m_pLastMade));

	// TODO vector way
	//// Register the objects
	//m_objContainer.push_back(m_pLastMade);
}

void ObjectManager::RemoveObject(const char* _name)
{
	// Find the one to remove
	auto toRemove = m_objContainer.find(_name);

	// If found the one
	if (toRemove != m_objContainer.end()) {
		delete (*toRemove).second;;
	}

	// Unless...
	else
		JE_DEBUG_PRINT("No such object\n");

	// TODO: Under the vector

	//// Defualt toggle
	//bool found = false;

	//for (auto it : m_objContainer) {

	//	// If found the one
	//	if (!strcmp(_name, it->m_name.c_str())) {
	//		
	//		// Remove all child objects belong to this object
	//		for (auto child : it->m_childObjs)
	//			ObjectManager::RemoveObject(child->GetName().c_str());

	//		// Remove all components belong to this object
	//		for (auto component : it->m_cptContainer)
	//			ComponentManager::RemoveComponent(component);

	//		found = true;	// Set toggle
	//		delete it;		// Remove the object
	//		break;			
	//	}
	//}

	//// If couldn't find the one,
	//if (!found)
	//	JE_DEBUG_PRINT("No such object\n");
}

Object* ObjectManager::GetObject(const char * _name)
{
	// Find the one to remove
	auto toRemove = m_objContainer.find(_name);

	// If found the one
	if (toRemove != m_objContainer.end())
		return (*toRemove).second;

	// Unless...
	else {
		JE_DEBUG_PRINT("No such object\n");
		return nullptr;
	}

	// TODO: Under the vector
	//for (auto it : m_objContainer) {

	//	// If it has the one,
	//	// return true
	//	if (!strcmp(_name, it->m_name.c_str()))
	//		return it;
	//}

	//// Unless,
	//JE_DEBUG_PRINT("No such Object\n");
	//return nullptr;
}

ObjectContainer* ObjectManager::GetObjectContainer()
{
	return &m_objContainer;
}

bool ObjectManager::HasObject(const char* _name)
{
	// Find the one to remove
	auto toRemove = m_objContainer.find(_name);

	// If found the one
	if (toRemove != m_objContainer.end()) {
		return true;
	}

	// Unless...
	return false;

	// TODO: Under the vector way
	//for (auto it : m_objContainer) {
	//	
	//	// If it has the one,
	//	// return true
	//	if(!strcmp(_name, it->m_name.c_str()))
	//		return true;
	//}

	//// Unless,
	//return false;
}

NS_JE_END

