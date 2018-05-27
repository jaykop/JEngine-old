#pragma once
#include "Macro.h"

jeBegin

class Object;
class Component;

class ComponentBuilder {

	friend class AssetManager;
	friend class ComponentManager;

	ComponentBuilder(ComponentBuilder&&) = delete;
	ComponentBuilder(const ComponentBuilder&) = delete;
	ComponentBuilder& operator=(ComponentBuilder&&) = delete;
	ComponentBuilder& operator=(const ComponentBuilder&) = delete;

public:

	virtual Component*	CreateComponent(Object* _pOwner) const = 0;

#ifdef  jeUseBuiltInAllocator
        virtual Component*	CreateComponent(Object* _pOwner, const char* _typeName) const = 0;
	virtual void		RemoveComponent(Component* _pComponent) = 0;
#endif // jeUseBuiltInAllocator

protected:

	ComponentBuilder() {};
	~ComponentBuilder() {};

};

#ifdef  jeUseBuiltInAllocator

#define jeBaseFriends(c)                        template <class T> friend class MemoryAllocator; friend class jeConcat(c, Builder); friend class ComponentManager;
#define jeDefineComponentBuilder(c)		MemoryAllocator<c> jeConcat(c, Builder)::m_allocator; jeConcat(c, Builder)::jeConcat(c, Builder)() {} Component* jeConcat(c, Builder)::CreateComponent(Object* _pOwner, const char* _typeName) const { c* toReturn = new (m_allocator.Allocate()) c(_pOwner); toReturn->m_typeName.assign(_typeName); return toReturn;/*return new (c)(_pOwner);*/} void jeConcat(c, Builder)::RemoveComponent(Component* _pComponent) { m_allocator.Free(static_cast<c*>(_pComponent)); }
#define jeDeclareComponentBuilder(c)		class jeConcat(c, Builder) : public ComponentBuilder { template <class ComponentType> friend class MemoryAllocator; friend class AssetManager; static MemoryAllocator<c> m_allocator; jeConcat(c, Builder)();	~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; Component* CreateComponent(Object* _pOwner, const char* _typeName) const override; void RemoveComponent(Component* _pComponent) override; }
#define jeDefineCustomComponentBuilder(c)	MemoryAllocator<c> jeConcat(c, Builder)::m_allocator; jeConcat(c, Builder)::jeConcat(c, Builder)() {} CustomComponent* jeConcat(c, Builder)::CreateComponent(Object* _pOwner, const char* _typeName) const { c* toReturn = new (m_allocator.Allocate()) c(_pOwner); toReturn->m_typeName.assign(_typeName); return toReturn;/*return new (c)(_pOwner);*/} void jeConcat(c, Builder)::RemoveComponent(Component* _pComponent) { m_allocator.Free(static_cast<c*>(_pComponent)); /*TODO*/; }
#define jeDeclareCustomComponentBuilder(c)	class jeConcat(c, Builder) : public ComponentBuilder { template <class ComponentType> friend class MemoryAllocator; friend class JEngine; static MemoryAllocator<c> m_allocator; jeConcat(c, Builder)(); ~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; CustomComponent* CreateComponent(Object* _pOwner, const char* _typeName) const override; void RemoveComponent(Component* _pComponent) override; }

#else

#define jeBaseFriends(c)                        friend class jeConcat(c, Builder); friend class ComponentManager;
#define jeDefineComponentBuilder(c)		jeConcat(c, Builder)::jeConcat(c, Builder)() {} Component* jeConcat(c, Builder)::CreateComponent(Object* _pOwner) const { return new (c)(_pOwner);}
#define jeDeclareComponentBuilder(c)		class jeConcat(c, Builder) : public ComponentBuilder { friend class AssetManager; jeConcat(c, Builder)(); ~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; Component* CreateComponent(Object* _pOwner) const override; }
#define jeDefineCustomComponentBuilder(c)	jeConcat(c, Builder)::jeConcat(c, Builder)() {} CustomComponent* jeConcat(c, Builder)::CreateComponent(Object* _pOwner) const { return new (c)(_pOwner); } 
#define jeDeclareCustomComponentBuilder(c)	class jeConcat(c, Builder) : public ComponentBuilder { friend class JEngine; jeConcat(c, Builder)(); ~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; CustomComponent* CreateComponent(Object* _pOwner) const override; }

#endif // jeUseBuiltInAllocator

jeEnd
