#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Light;
class LightControllerBuilder : public ComponentBuilder
{

	friend class Core;

public:

private:

	LightControllerBuilder();
	~LightControllerBuilder() {};
	LightControllerBuilder(const LightControllerBuilder& _copy) = delete;
	void operator=(const LightControllerBuilder& _copy) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class LightController : public CustomComponent
{
	friend class ComponentManager;
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

	void EditorUpdate(const float _dt) override;
};

JE_END