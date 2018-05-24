#pragma once
#include "Macro.h"

jeBegin

template <class T>
class MemoryAllocator;

class ObjectFactory {

	friend class Object;
	friend class ObjectContainer;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(ObjectFactory)

	static MemoryAllocator<Object> allocator;

public:

	static void		CreateObject(const char* _name);
	static Object*	GetCreatedObject();
	static void		AddCreatedObject();

private:

	static bool		m_added;
	static unsigned	m_registerNumber;
	static Object*	m_pLastMade;

};

using FACTORY = ObjectFactory;

jeEnd
