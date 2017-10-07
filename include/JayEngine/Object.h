#pragma once

#include "Macro.h"
#include <iostream>
#include <unordered_map>

NS_JE_BEGIN

class	Object;
class	Component;

typedef std::unordered_map<std::string, Object*> ChildObjects;
typedef std::unordered_map<std::string, Component*>	ComponentMap;

class Object {

public:

	friend class ObjectManager;

	Object(const char* _name);
	~Object();

	unsigned GetId() const;

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

	template <typename ComponentType>
	inline void				AddComponent();

	template <typename ComponentType>
	inline ComponentType*	GetComponent();

	template <typename ComponentType>
	inline bool				HasComponent();

private:

	Object() {};
	Object(const Object& /*_copy*/) {};
	void operator=(const Object& /*_copy*/) {};

	// Member function
	void SetParent(Object* _pObject); 
	
	void ClearComponents();
	void ClearChildren();

	unsigned		m_id;
	bool			m_active;
	Object*			m_pParent;
	std::string		m_name;
	ChildObjects	m_childObjs;
	ComponentMap	m_componentMap;
};

NS_JE_END

#include "Object.inl"