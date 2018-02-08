#pragma once
#include "UserComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Light;
class LightControllerBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	LightControllerBuilder();
	~LightControllerBuilder() {};
	LightControllerBuilder(const LightControllerBuilder& _copy) = delete;
	void operator=(const LightControllerBuilder& _copy) = delete;

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class LightController : public UserComponent
{

	friend class LightControllerBuilder;

public:

private:

	LightController(Object* pObject);
	~LightController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

	Light* m_light;
};

JE_END