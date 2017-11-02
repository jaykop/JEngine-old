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
	TransformBuilder(const TransformBuilder& /*_copy*/) {};
	void operator=(const TransformBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;
};

class Transform : public Component
{

	struct Edge {
		Vector3 m_leftTop, m_rightTop, m_leftBot, m_rightBot;
	};

	friend class PhysicsSystem;
	friend class ComponentManager;
	friend class TransformBuilder;

public:

	vec3 m_position, m_scale, m_rotation3D;
	float m_rotation;
	Edge m_edges;

private:

	Transform(Object* _owner = nullptr);
	~Transform() {};
	Transform(const Transform& /*_copy*/) {};
	void operator= (const Transform& /*_copy*/) {};

	void Load(const RJValue& _data) override;
	void Init() override {};
	void Update(float /*_dt*/) override {};
	void Close() override {};
	void Unload() override {};

};

JE_END
