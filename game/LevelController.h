#pragma once
#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

struct Telegram;
class Camera;

class LevelController : public CustomComponent
{
	template <class T>
	friend class MemoryAllocator;

	friend class ComponentManager;
	friend class LevelControllerBuilder;

public:

private:

	LevelController(Object* pObject);
	~LevelController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;
	bool OnMessage(Telegram& msg) override { msg;  return false; }

	void EditorUpdate(float dt) override;
};

jeDeclareCustomComponentBuilder(LevelController);
jeEnd
