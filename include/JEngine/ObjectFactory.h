#pragma once
#include "Macro.h"

JE_BEGIN

class ObjectFactory {

	friend class Object;

	// Locked constuctor, destructor, assign operator
	JE_THIS_IS_STATIC_CLASS(ObjectFactory)

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

JE_END
