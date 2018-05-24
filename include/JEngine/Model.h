#pragma once
#include "Sprite.h"

jeBegin
jeDeclareComponentBuilder(Model);

class Model : public Sprite
{
	template <class T>
	friend class MemoryAllocator;

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
