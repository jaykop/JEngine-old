#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "ComponentManager.h"

jeBegin

class Light;
struct Telegram;

class LightController : public CustomComponent
{
	template <class T>
	friend class MemoryAllocator;

	friend class ComponentManager;
	friend class LightControllerBuilder;

public:

private:

	LightController(Object* pObject);
	~LightController() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;
	bool OnMessage(Telegram& msg) override { msg; return false; }

	Light* m_light;

	void EditorUpdate(float dt) override;
};

jeDeclareCustomComponentBuilder(LightController);
jeEnd
