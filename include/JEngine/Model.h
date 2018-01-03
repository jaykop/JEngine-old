#pragma once
#include "Sprite.h"

JE_BEGIN

class ModelBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	ModelBuilder();
	~ModelBuilder() {};
	ModelBuilder(const ModelBuilder& /*_copy*/) = delete;
	void operator=(const ModelBuilder& /*_copy*/) = delete;

	Component* CreateComponent(Object* _pOwner) const override;

};

class Model : public Sprite
{
	friend class GraphicSystem;
	friend class ComponentManager;
	friend class ModelBuilder;

public:

private:

	Model(Object* pObject);
	~Model() {};

	Model() = delete;
	Model(const Model& /*_copy*/) = delete;
	void operator=(const Model& /*_copy*/) = delete;
};

JE_END