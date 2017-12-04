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
	LightBuilder(const LightBuilder& /*_copy*/) = delete;
	void operator=(const LightBuilder& /*_copy*/) = delete;

	Component* CreateComponent(Object* _pOwner) const override;

};

class Light : public Component
{

	enum LightType {NORMALLIGHT, DIRECTIONALLIGHT, SPOTLIGHT, POINTLIGHT};

	friend class GraphicSystem;
	friend class ComponentManager;
	friend class LightBuilder;

public:

	LightType	m_type;
	vec3		m_position, m_direction;
	vec4		m_ambient, m_specular, m_diffuse, m_color;
	float		m_constant, m_linear, m_quadratic, m_cutOff, m_outerCutOff;

	void Register() override;

private:

	Light(Object* _owner);
	~Light() {};

	Light() = delete;
	Light(const Light& /*_copy*/) = delete;
	void operator=(const Light& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;

};

JE_END