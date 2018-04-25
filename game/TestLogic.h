#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Object;
class TestLogicBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	TestLogicBuilder();
	~TestLogicBuilder() {};
	TestLogicBuilder(const TestLogicBuilder& /*_copy*/) = delete;
	void operator=(const TestLogicBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

void threadTest(int a);
class TestLogic : public CustomComponent
{
	friend class ComponentManager;
	friend class TestLogicBuilder;

public:

	Object *m_ortho, *m_pers;

private:

	TestLogic(Object* pObject);
	~TestLogic() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

	void EditorUpdate(const float _dt) override;

};

JE_END