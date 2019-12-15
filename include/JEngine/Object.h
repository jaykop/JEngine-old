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
	    CustomComponent* pPreviousState_ = nullptr;
	    CustomComponent* pCurrentState_ = nullptr;
	    CustomComponent* pGlobalState_ = nullptr;
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

	Object(const char* name);
	~Object();

	unsigned GetId() const;

	void RegisterComponents();

	const std::string&  GetName(void) const;
	void				SetName(const char* name);

	void	AddChild(Object* child);
	void	RemoveChild(const char* name);
	Object* GetChild(const char* name);
	bool	HasChild(const char* name);
	
	Object*	GetParent();
	bool	HasParent();

	void	SetActive(bool active);
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

	void	    AddComponent(const char* componentName);
	Component*  GetComponent(const char* componentName);
	bool	    HasComponent(const char* componentName) const;
	void	    RemoveComponent(const char* componentName);

	template<typename ComponentType>
	inline void SetGlobalState();
	
	template<typename ComponentType>
	inline void SetCurrentState();
	
	template<typename ComponentType>
	inline void ChangeState();
	
	CustomComponent* GetGlobalState() const;
	CustomComponent* GetCurrentState() const;

	void SetGlobalState(const char* componentName);
	void SetCurrentState(const char* componentName);
	void RevertToPreviousState();

private:

	// Member function
	void SetParent(Object* pObject); 
	
	void ClearComponents();
	void ClearChildren();

	bool HandleMessage(Telegram& _message);

	unsigned		id_;
	bool			active_;
	Object*			pParent_;
	StateMachine	stateMachine_;
	std::string		name_;
	ChildObjects	childObjs_;
	ComponentMap	componentMap_;
	
	// statiic editor variable and function
	void	EditorUpdate(float dt);
	bool	showEditor_;

	Object() = delete;
	Object(const Object& /*copy*/) = delete;
	void operator=(const Object& /*copy*/) = delete;

};

jeEnd

#include "Object.inl"