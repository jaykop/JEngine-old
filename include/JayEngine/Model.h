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
	ModelBuilder(const ModelBuilder& /*_copy*/) {};
	void operator=(const ModelBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};

class Model : public Sprite
{
	friend class GraphicSystem;
	friend class ComponentManager;
	friend class ModelBuilder;

public:

private:

	Model(Object* pObject = nullptr);
	~Model() {};
	Model(const Model& /*_copy*/) {};
	void operator=(const Model& /*_copy*/) {};
};

JE_END