#pragma once
#include "Macro.h"

JE_BEGIN

class ObjectFactory {

	friend class Object;

public:

	static void		CreateObject(const char* _name);
	static Object*	GetCreatedObject();
	static void		AddCreatedObject(ObjectContainer* _container);

private:

	~ObjectFactory() {};
	ObjectFactory() {};
	ObjectFactory(const ObjectFactory& /*_copy*/) = delete;
	void operator=(const ObjectFactory& /*_copy*/) = delete;

	static bool			m_added;
	static unsigned		m_registerNumber;
	static Object*		m_pLastMade;

};

using FACTORY = ObjectFactory;

JE_END
