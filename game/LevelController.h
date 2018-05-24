#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

jeBegin

jeDeclareCustomComponentBuilder(LevelController);

struct Telegram;
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

jeEnd
