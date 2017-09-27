#pragma once

#include <iostream>
#include "ComponentContainer.h"

NS_JE_BEGIN

class	Object;
class	Component;
typedef std::unordered_map<std::string, Object*> ChildObjects;

class Object {

public:

	friend class ObjectManager;

	Object(const std::string& _name);
	~Object();

	const std::string&	GetName(void) const;
	void				SetName(const char* _name);

	void	AddChild(Object* _child);
	void	RemoveChild(const char* _name);
	Object* GetChild(const char* _name);
	bool	HasChild(const char* _name);

	void	SetActive(bool _active);
	bool	GetActive(void) const;

	ComponentContainer* GetComponentMap();

private:

	Object() {};
	Object(const Object& /*_copy*/) {};

	void operator=(const Object& /*_copy*/) {};

	bool				m_active;
	Object*				m_pParent;
	std::string			m_name;
	ChildObjects		m_childObjs;
	ComponentContainer	m_cptContainer;
};

NS_JE_END