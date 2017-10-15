#pragma once
#include "Macro.h"

NS_JE_BEGIN

class ObjectFactory {

	friend class Object;

public:

	static void		CreateObject(const char* _name);
	static Object*	GetCreatedObject();
	static void		AddCreatedObject(ObjectContainer* _container);

private:

	~ObjectFactory() {};
	ObjectFactory() {};
	ObjectFactory(const ObjectFactory& /*_copy*/) {};
	void operator=(const ObjectFactory& /*_copy*/) {};

	static bool			m_added;
	static unsigned		m_registerNumber;
	static Object*		m_pLastMade;

};

NS_JE_END