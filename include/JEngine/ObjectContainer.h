#pragma once
#include "Macro.h"
#include <unordered_map>

jeBegin

class Object;
using ObjectMap = std::unordered_map<std::string, Object*>;

class ObjectContainer {

	friend class State;
	friend class StateManager;
	friend class ObjectFactory;

	ObjectContainer(ObjectContainer&&) = delete;
	ObjectContainer(const ObjectContainer&) = delete;
	ObjectContainer& operator=(ObjectContainer&&) = delete;
	ObjectContainer& operator=(const ObjectContainer&) = delete;

public:

	void		RemoveObject(Object* pObj);
	void		RemoveObject(const char* name);
	void		RemoveObject(unsigned id);
	Object*		GetObject(const char* name);
	Object*		GetObject(unsigned id);
	bool		HasObject(const char* name);
	bool		HasObject(unsigned id);
	ObjectMap&	GetObjectMap();

	template <typename ComponentType>
	inline ObjectMap        GetObjects();

	static ObjectContainer* pContainer_;
	
private:

	~ObjectContainer();
	ObjectContainer() {};

	void ClearObjectMap();

	// Static variable and function
	static void		EditorUpdate(float dt);

	ObjectMap	objectMap_;
};

using OBJECT = ObjectContainer;

jeEnd

#include "ObjectContainer.inl"

#define jeAddComponent(o, c)	GetObject(o)->AddComponent<c>()
#define jeGetComponent(o, c)	GetObject(o)->GetComponent<c>()
#define jeRemoveComponent(o, c)	GetObject(o)->RemoveComponent<c>()
#define jeHasComponent(o, c)	GetObject(o)->HasComponent<c>()
