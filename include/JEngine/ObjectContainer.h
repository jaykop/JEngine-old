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

	// These functions are able to
	// connect to objects directly
	template <typename ComponentType>
	inline void				AddComponent(const char* _toObject);

	template <typename ComponentType>
	inline ComponentType*	GetComponent(const char* _fromObject);

	template <typename ComponentType>
	inline bool				HasComponent(const char* _fromObject);

	template <typename ComponentType>
	inline void				RemoveComponent(const char* _fromObject);

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

JE_END

#include "ObjectContainer.inl"
