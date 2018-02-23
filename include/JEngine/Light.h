#pragma once
#include "Component.h"
#include "Vector3.h"
#include "Vector4.h"
#include "ComponentBuilder.h"

// For enum ProjectType
#include "GraphicSystem.h"

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

	enum LightType  {NORMALLIGHT, DIRECTIONALLIGHT, SPOTLIGHT, POINTLIGHT};

	friend class ComponentManager;
	friend class GraphicSystem;
	friend class LightBuilder;

public:

	LightType	m_type;
	ProjectType	m_projection;

	vec3		m_position, m_direction;
	vec4		m_ambient, m_specular, m_diffuse, m_color;
	float		m_constant, m_linear, m_quadratic, m_cutOff, m_outerCutOff;

	void Register() override;

private:

	Light(Object* _pOwner);
	~Light();
	void operator=(const Light& _copy);

	Light() = delete;
	Light(const Light& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;

	static void		EditorUpdate(const float _dt);
	static bool		m_showWindow;
	static Light*	m_pEdit;
};

JE_END