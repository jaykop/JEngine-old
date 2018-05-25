#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Transform : public Component
{
	template <class T>
	friend class MemoryAllocator;

	friend class ComponentManager;
	friend class PhysicsSystem;
	friend class TransformBuilder;

public:

	vec3	m_position, m_scale, m_rotationAxis;
	float	m_rotation;

private:

	Transform(Object* _pOwner);
	~Transform() {};
	void operator= (const Transform& _copy);

	void Load(CR_RJValue _data) override;
	void Register() override {};

	void EditorUpdate(const float _dt) override;

	Transform() = delete;
	Transform(const Transform& /*_copy*/) = delete;

};

jeDeclareComponentBuilder(Transform);
jeEnd
