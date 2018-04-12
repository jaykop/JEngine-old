#pragma once

#include "Macro.h"
#include <unordered_map>

JE_BEGIN

class Object;
class Component;

using ChildObjects = std::unordered_map<std::string, Object*>;
using ComponentMap = std::unordered_map<std::string, Component*>;

class Object {

public:

	friend class JsonParser;
	friend class ObjectFactory;
	friend class ObjectContainer;
	friend class ImguiManager;

	Object(const char* _name);
	~Object();

	unsigned GetId() const;

	void RegisterComponents();

	const std::string&	GetName(void) const;
	void				SetName(const char* _name);

	void	AddChild(Object* _child);
	void	RemoveChild(const char* _name);
	Object* GetChild(const char* _name);
	bool	HasChild(const char* _name);
	
	Object*	GetParent();
	bool	HasParent();

	void	SetActive(bool _active);
	bool	GetActive(void) const;

	ComponentMap& GetComponentMap();

	template<typename ComponentType>
	inline void				AddComponent();

	template <typename ComponentType>
	inline ComponentType*	GetComponent();

	template<typename ComponentType>
	inline bool				HasComponent();

	template<typename ComponentType>
	inline void				RemoveComponent();

private:

	// Member function
	void SetParent(Object* _pObject); 
	
	void ClearComponents();
	void ClearChildren();

	void		AddComponent(const char* _componentName);
	Component*	GetComponent(const char* _componentName);
	bool		HasComponent(const char* _componentName) const;
	void		RemoveComponent(const char* _componentName);

	unsigned			m_id;
	bool				m_active;
	Object*				m_pParent;
	std::string			m_name;
	ChildObjects		m_childObjs;
	ComponentMap		m_componentMap;
	ObjectContainer*	m_pOBC;
	
	// statiic editor variable and function
	void	EditorUpdate(const float _dt);
	bool	m_showEditor;

	Object() = delete;
	Object(const Object& /*_copy*/) = delete;
	void operator=(const Object& /*_copy*/) = delete;

};

JE_END

#include "Object.inl"