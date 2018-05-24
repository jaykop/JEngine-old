#pragma once
#include "Macro.h"

jeBegin

class ObjectFactory {

	friend class Object;

	// Locked constuctor, destructor, assign operator
	jeStaticClassDeclaration(ObjectFactory)

public:

	static void		CreateObject(const char* _name);
	static Object*	GetCreatedObject();
	static void		AddCreatedObject();

private:


	static bool			m_added;
	static unsigned		m_registerNumber;
	static Object*		m_pLastMade;

};

using FACTORY = ObjectFactory;

jeEnd
