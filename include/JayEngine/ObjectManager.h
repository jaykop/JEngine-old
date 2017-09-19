#pragma once
#include "Macro.h"
#include <unordered_map>

NS_JE_BEGIN

class Object;
typedef std::unordered_map<std::string, Object*> ObjectContainer;

class ObjectManager {

public:

	static void				CreateObject(const char* _name);
	static Object*			GetCreatedObject();
	static void				AddCreatedObject();

	static void				RemoveObject(const char* _name);
	static Object*			GetObject(const char* _name);
	static ObjectContainer*	GetObjectContainer();

	static bool				HasObject(const char* _name);

private:

	ObjectManager() {};
	~ObjectManager() {};
	ObjectManager(const ObjectManager& /*_copy*/) {};
	void operator=(const ObjectManager& /*_copy*/) {};

	static Object*			m_pLastMade;
	static ObjectContainer	m_objContainer;
};

NS_JE_END