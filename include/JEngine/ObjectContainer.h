#pragma once
#include "Macro.h"
#include <unordered_map>

JE_BEGIN

class Object;
using ObjectMap = std::unordered_map<std::string, Object*>;

class ObjectContainer {

	friend class State;
	friend class StateManager;
	friend class ObjectFactory;

public:

	void		RemoveObject(const char* _name);
	void		RemoveObject(unsigned _id);
	Object*		GetObject(const char* _name);
	Object*		GetObject(unsigned _id);
	bool		HasObject(const char* _name);
	bool		HasObject(unsigned _id);
	ObjectMap&	GetObjectMap();

	template <typename ComponentType>
	inline ObjectMap		GetObjects();

	static ObjectContainer* GetCurrentContainer();

private:

	~ObjectContainer();
	ObjectContainer() {};
	ObjectContainer(const ObjectContainer& /*_copy*/) = delete;
	void operator=(const ObjectContainer& /*_copy*/) = delete;

	void ClearObjectMap();

	// Static variable and function
	static ObjectContainer* m_pSharedContainer;
	static void				EditorUpdate(const float _dt);

	ObjectMap	m_objectMap;
};

using OBJECT = ObjectContainer;
#define CONTAINER OBJECT::GetCurrentContainer()

JE_END

#include "ObjectContainer.inl"

#define ADD_COMPONENT(o, c)		GetObject(o)->AddComponent<c>()
#define GET_COMPONENT(o, c)		GetObject(o)->GetComponent<c>()
#define REMOVE_COMPONENT(o, c)	GetObject(o)->RemoveComponent<c>()
#define HAS_COMPONENT(o, c)		GetObject(o)->HasComponent<c>()
