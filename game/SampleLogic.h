#pragma once
#include "Component.h"
#include "ComponentBuilder.h"

JE_BEGIN

class SampleLogicBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	SampleLogicBuilder();
	~SampleLogicBuilder() {};
	SampleLogicBuilder(const SampleLogicBuilder& /*_copy*/) {};
	void operator=(const SampleLogicBuilder& /*_copy*/) {};

	Component* CreateComponent(Object* _pOwner) const override;

};

class SampleLogic : public Component
{
	friend class ComponentManager;

public:

	void Load(const RJValue& _data) override;
	void Init() override;
	void Update(float _dt) override;
	void Close() override;
	void Unload() override;

private:

	SampleLogic() {};
	~SampleLogic() {};
	SampleLogic(const SampleLogic& /*_copy*/) {};
	void operator=(const SampleLogic& /*_copy*/) {};

};

JE_END