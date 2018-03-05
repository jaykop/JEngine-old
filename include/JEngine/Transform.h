#pragma once
#include "Vector3.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class TransformBuilder : public ComponentBuilder
{
	
	friend class AssetManager;

public:
	

private:

	TransformBuilder();
	~TransformBuilder() {};
	TransformBuilder(const TransformBuilder& /*_copy*/) = delete;
	void operator=(const TransformBuilder& /*_copy*/) = delete;

	Component* CreateComponent(Object* _pOwner) const override;
};

class Transform : public Component
{
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

JE_END
