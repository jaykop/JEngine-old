#include "Object.h"
#include "ObjectFactory.h"
#include "ObjectContainer.h"

JE_BEGIN

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
Object*		ObjectFactory::m_pLastMade = nullptr;
unsigned	ObjectFactory::m_registerNumber = 0;
bool		ObjectFactory::m_added = true;

void ObjectFactory::CreateObject(const char* _name)
{
	// If there is ex-created and non-added object 
	// give user warning
	if (!m_added)
		JE_DEBUG_PRINT("There is a already created but not added object.\n");

	// unless just make new object
	else {
		m_added = false;
		m_pLastMade = new Object(_name);
	}
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

JE_END
