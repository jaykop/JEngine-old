#pragma once
#include "UserComponent.h"
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

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class SampleLogic : public UserComponent
{
	friend class ComponentManager;
	friend class SampleLogicBuilder;

public:

private:

	SampleLogic(Object* pObject = nullptr);
	~SampleLogic() {};
	SampleLogic(const SampleLogic& /*_copy*/) {};
	void operator=(const SampleLogic& /*_copy*/) {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float _dt) override;
	void Close() override;
	void Unload() override;
};

JE_END