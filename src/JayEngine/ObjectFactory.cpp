#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"

NS_JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
Object*		ObjectFactory::m_pLastMade = nullptr;
unsigned	ObjectFactory::m_registerNumber = 0;
bool		ObjectFactory::m_added = true;

void ObjectFactory::CreateObject(const char* _name)
{
	// If there is ex-created and non-added object 
	// to the list in advance, remove it
	if (!m_added) {
		delete m_pLastMade;
		m_pLastMade = nullptr;
	}

	else
		m_added = false;

	// Make new object
	m_pLastMade = new Object(_name);
}

Object* ObjectFactory::GetCreatedObject()
{
	// Return the lastly created one
	return m_pLastMade;
}

void ObjectFactory::AddCreatedObject(ObjectContainer* _container)
{
	// Register the object
	_container->m_objectMap.insert(
		ObjectMap::value_type(
			m_pLastMade->m_name, m_pLastMade));
	m_pLastMade->m_pOBC = _container;

	m_added = true;
	++m_registerNumber;
}

NS_JE_END