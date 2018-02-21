#pragma once
#include "Macro.h"
#include <unordered_map>

JE_BEGIN

class Object;
using ObjectMap = std::unordered_map<std::string, Object*>;

class ObjectContainer {

	friend class State;
	friend class ObjectFactory;

public:

	void		RemoveObject(const char* _name);
	Object*		GetObject(const char* _name);
	ObjectMap&	GetObjectMap();
	bool		HasObject(const char* _name);

	// These functions are able to
	// connect to objects directly
	template <typename ComponentType>
	inline void				AddComponent(const char* _toObject);

	template <typename ComponentType>
	inline ComponentType*	GetComponent(const char* _fromObject);

	template <typename ComponentType>
	inline bool				HasComponen(const char* _fromObject);

	template <typename ComponentType>
	inline void				RemoveComponent(const char* _fromObject);

	template <typename ComponentType>
	inline ObjectMap		GetObjects();

private:

	~ObjectContainer();
	ObjectContainer() {};
	ObjectContainer(const ObjectContainer& /*_copy*/) = delete;
	void operator=(const ObjectContainer& /*_copy*/) = delete;

	void		ClearObjectMap();
	void		EditorUpdate(const float _dt);

	ObjectMap	m_objectMap;
};

JE_END

#include "ObjectContainer.inl"
