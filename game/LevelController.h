#pragma once
#include "UserComponent.h"
#include "ComponentBuilder.h"

#include "Vector3.h"

JE_BEGIN

class LevelControllerBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	LevelControllerBuilder();
	~LevelControllerBuilder() {};
	LevelControllerBuilder(const LevelControllerBuilder& _copy) = delete;
	void operator=(const LevelControllerBuilder& _copy) = delete;

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class Camera;
class LevelController : public UserComponent
{

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
};

JE_END