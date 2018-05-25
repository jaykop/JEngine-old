#pragma once
#include "JsonParser.h"

jeBegin

class Component {

	friend class Object;
	friend class JsonParser;
	friend class ImguiManager;
	friend class ComponentManager;

public:

	virtual void Register() = 0;

protected:

	Component(Object* _pOwner, bool _byUser = false)
		: m_pOwner(_pOwner), m_byUser(_byUser), 
		m_showEditor(false), m_typeName("Component") {};
	virtual	~Component() {};
	void operator=(const Component& _copy) {
		m_byUser = _copy.m_byUser;
	};

	virtual void Load(CR_RJValue _data) = 0;
	virtual void EditorUpdate(float _dt) = 0;

	Object*		m_pOwner;
	bool		m_byUser, m_showEditor;
	std::string m_typeName;

private:

	Component() = delete;;
	Component(const Component& /*_copy*/) = delete;

};

#define jeDeclareStaticAllocator(x)	template <class T> friend class MemoryAllocator; static MemoryAllocator<x> allocator
#define jeDefineStaticAllocator(x)	MemoryAllocator<x> x::allocator

jeEnd
