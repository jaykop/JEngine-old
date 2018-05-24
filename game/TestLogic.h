#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

jeBegin

class Object;
struct Telegram;

jeDeclareCustomComponentBuilder(TestLogic);

class TestLogic : public CustomComponent
{
	friend class ComponentManager;
	friend class TestLogicBuilder;

public:

	Object *m_ortho, *m_pers;

private:

	TestLogic(Object* pObject);
	~TestLogic() {};

	static int a;

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;
	bool OnMessage(Telegram& msg) override { msg; return false; }

	void EditorUpdate(const float _dt) override;

};

jeEnd
