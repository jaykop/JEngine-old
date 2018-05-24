#pragma once
#include "Sprite.h"

jeBegin

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
	jeDeclareStaticAllocator(Model);
	friend class ComponentManager;
	friend class GraphicSystem;
	friend class ModelBuilder;

public:

private:

	Model(Object* pObject);
	~Model();
	void operator=(const Model& _copy);

	Model() = delete;
	Model(const Model& /*_copy*/) = delete;

	void EditorUpdate(const float _dt) override;
};

jeEnd
