#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

jeBegin

class Light;
struct Telegram;

jeDeclareCustomComponentBuilder(LightController);

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
	bool OnMessage(Telegram& msg) override { msg; return false; }

	Light* m_light;

	void EditorUpdate(const float _dt) override;
};

jeEnd
