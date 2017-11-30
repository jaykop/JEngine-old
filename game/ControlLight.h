#pragma once
#include "UserComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Light;
class ControlLightBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	ControlLightBuilder();
	~ControlLightBuilder() {};
	ControlLightBuilder(const ControlLightBuilder& /*_copy*/) {};
	void operator=(const ControlLightBuilder& /*_copy*/) {};

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class ControlLight : public UserComponent
{
	friend class ComponentManager;
	friend class ControlLightBuilder;

public:

private:

	ControlLight(Object* pObject);
	~ControlLight() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float _dt) override;
	void Close() override;
	void Unload() override;

	Light* m_light;
};

JE_END