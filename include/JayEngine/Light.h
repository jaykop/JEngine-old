#pragma once
#include "Macro.h"
#include "Component.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ComponentBuilder.h"

JE_BEGIN

class LightBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	LightBuilder();
	~LightBuilder() {};
	LightBuilder(const LightBuilder& /*_copy*/) {};
	void operator=(const LightBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};

class Light : public Component
{

	friend class GraphicSystem;
	friend class ComponentManager;
	friend class LightBuilder;

public:

	vec3		m_position;
	vec4		m_ambient, m_specular, m_diffuse, m_color;

	void Register() override;

private:

	Light(Object* _owner = nullptr);
	~Light() {};
	Light(const Light& /*_copy*/) {};
	void operator=(const Light& /*_copy*/) {};

	void Load(CR_RJValue _data) override;

};

JE_END