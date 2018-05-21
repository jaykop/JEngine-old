#include "ObjectFactory.h"
#include "ObjectContainer.h"
#include "ImguiManager.h"

jeBegin

//////////////////////////////////////////////////////////////////////////
// static variables
//////////////////////////////////////////////////////////////////////////
Object*		FACTORY::m_pLastMade = nullptr;
unsigned	FACTORY::m_registerNumber = 0;
bool		FACTORY::m_added = true;

void ObjectFactory::CreateObject(const char* _name)
{
	// If there is ex-created and non-added object 
	// give user warning
	if (!m_added)
		jeDebugPrint("!ObjectFactory - Holding not added object yet: %s\n", _name);

	// unless just make new object
	else {
		m_added = false;
		++m_registerNumber;
		m_pLastMade = new Object(_name);
	}
}

Object* ObjectFactory::GetCreatedObject()
{
	// Return the lastly created one
	return m_pLastMade;
}

void ObjectFactory::AddCreatedObject()
{
	// Register the object
	CONTAINER->m_objectMap.insert(
		ObjectMap::value_type(
			m_pLastMade->m_name, m_pLastMade));
	IMGUI::AddObjectEditor(m_pLastMade);
	m_pLastMade->RegisterComponents();
	m_added = true;
}

jeEnd
