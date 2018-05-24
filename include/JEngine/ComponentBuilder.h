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
	virtual void		RemoveComponent(Component* _pComponent) = 0;

protected:

	ComponentBuilder() {};
	~ComponentBuilder() {};

};

#define jeDefineComponentBuilder(c)			MemoryAllocator<c> jeConcat(c, Builder)::m_allocator; jeConcat(c, Builder)::jeConcat(c, Builder)() {} Component* jeConcat(c, Builder)::CreateComponent(Object* _pOwner) const { return new (m_allocator.Allocate()) c(_pOwner);/*return new (c)(_pOwner);*/} void jeConcat(c, Builder)::RemoveComponent(Component* /*_pComponent*/) { /*TODO*/; }
#define jeDeclareComponentBuilder(c)		class jeConcat(c, Builder) : public ComponentBuilder {	template <class ComponentType> friend class MemoryAllocator; friend class AssetManager; static MemoryAllocator<c> m_allocator; jeConcat(c, Builder)();	~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; Component* CreateComponent(Object* _pOwner) const override; void RemoveComponent(Component* _pComponent) override; }
#define jeDefineCustomComponentBuilder(c)	jeConcat(c, Builder)::jeConcat(c, Builder)() {} CustomComponent* jeConcat(c, Builder)::CreateComponent(Object* _pOwner) const {	return new (c)(_pOwner);} void jeConcat(c, Builder)::RemoveComponent(Component* /*_pComponent*/) { /*TODO*/; }
#define jeDeclareCustomComponentBuilder(c)	class jeConcat(c, Builder) : public ComponentBuilder {	friend class JEngine; jeConcat(c, Builder)();	~jeConcat(c, Builder)() {}; jeConcat(c, Builder)(const jeConcat(c, Builder)& /*_copy*/) = delete; void operator=(const jeConcat(c, Builder)& /*_copy*/) = delete; jeConcat(c, Builder)(jeConcat(c, Builder) && /*_copy*/) = delete; void operator=(jeConcat(c, Builder) && /*_copy*/) = delete; CustomComponent* CreateComponent(Object* _pOwner) const override; void RemoveComponent(Component* _pComponent) override; }

jeEnd
