#pragma once
#include "Macro.h"
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class MaterialBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	MaterialBuilder();
	~MaterialBuilder() {};
	MaterialBuilder(const MaterialBuilder& /*_copy*/) = delete;
	void operator=(const MaterialBuilder& /*_copy*/) = delete;

	Component* CreateComponent(Object* _pOwner) const override;

};

class Material : public Component {

	friend class MaterialBuilder;
	friend class GraphicSystem;
	friend class ComponentManager;

public:

	int m_diffuse, m_specular;
	float m_shininess;

private:

	Material(Object* _pOwner);
	~Material() {};

	Material() = delete;
	Material(const Material& /*_copy*/) = delete;
	void operator= (const Material& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;
	void Register() override {};
};

JE_END