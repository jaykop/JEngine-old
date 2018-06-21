#pragma once

#include "Macro.h"
#include <unordered_map>

jeBegin

struct Telegram;
class Object;
class Component;
class CustomComponent;

using ChildObjects = std::unordered_map<std::string, Object*>;
using ComponentMap = std::unordered_map<std::string, Component*>;

class Object {

	struct StateMachine
	{
	    CustomComponent* m_pPreviousState = nullptr;
	    CustomComponent* m_pCurrentState = nullptr;
	    CustomComponent* m_pGlobalState = nullptr;
	};

public:

#ifdef jeUseBuiltInAllocator
	template <class T>
	friend class MemoryAllocator;
#endif

	friend class JsonParser;
	friend class ObjectFactory;
	friend class ObjectContainer;
	friend class ImguiManager;
	friend class MessageDispatcher;

	Object(const char* _name);
	~Object();

	unsigned GetId() const;

	void RegisterComponents();

	const std::string&  GetName(void) const;
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
	inline void AddComponent();

	template <typename ComponentType>
	inline ComponentType*	GetComponent();

	template<typename ComponentType>
	inline bool HasComponent();

	template<typename ComponentType>
	inline void RemoveComponent();

	void	    AddComponent(const char* _componentName);
	Component*  GetComponent(const char* _componentName);
	bool	    HasComponent(const char* _componentName) const;
	void	    RemoveComponent(const char* _componentName);

	template<typename ComponentType>
	inline void SetGlobalState();
	
	template<typename ComponentType>
	inline void SetCurrentState();
	
	template<typename ComponentType>
	inline void ChangeState();
	
	CustomComponent* GetGlobalState() const;
	CustomComponent* GetCurrentState() const;

	void SetGlobalState(const char* _componentName);
	void SetCurrentState(const char* _componentName);
	void RevertToPreviousState();

private:

	// Member function
	void SetParent(Object* _pObject); 
	
	void ClearComponents();
	void ClearChildren();

	bool HandleMessage(Telegram& _message);

	unsigned		m_id;
	bool			m_active;
	Object*			m_pParent;
	StateMachine	m_StateMachine;
	std::string		m_name;
	ChildObjects	m_childObjs;
	ComponentMap	m_componentMap;
	
	// statiic editor variable and function
	void	EditorUpdate(const float _dt);
	bool	m_showEditor;

	Object() = delete;
	Object(const Object& /*_copy*/) = delete;
	void operator=(const Object& /*_copy*/) = delete;

};

jeEnd

#include "Object.inl"