#pragma once
#include "Component.h"
#include "ComponentBuilder.h"

jeBegin

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

	friend class ComponentManager;
	friend class MaterialBuilder;
	friend class GraphicSystem;

public:

	int m_diffuse, m_specular;
	float m_shininess;

private:

	Material(Object* _pOwner);
	~Material() {};
	void operator= (const Material& _copy);

	Material() = delete;
	Material(const Material& /*_copy*/) = delete;

	void Load(CR_RJValue _data) override;
	void Register() override {};

	void EditorUpdate(const float _dt) override;
};

jeEnd
