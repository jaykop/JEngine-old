#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

struct Telegram;

class LevelControllerBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	LevelControllerBuilder();
	~LevelControllerBuilder() {};
	LevelControllerBuilder(const LevelControllerBuilder& _copy) = delete;
	void operator=(const LevelControllerBuilder& _copy) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class Camera;
class LevelController : public CustomComponent
{
	friend class ComponentManager;
	friend class LevelControllerBuilder;

public:

private:

	LevelController(Object* pObject);
	~LevelController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;
	bool OnMessage(Telegram& msg) override { msg;  return false; }

	void EditorUpdate(const float _dt) override;
};

JE_END