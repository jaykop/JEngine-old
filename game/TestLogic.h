#pragma once
#include "UserComponent.h"
#include "ComponentBuilder.h"

JE_BEGIN

class Light;
class TestLogicBuilder : public ComponentBuilder
{

	friend class Core;

public:

private:

	TestLogicBuilder();
	~TestLogicBuilder() {};
	TestLogicBuilder(const TestLogicBuilder& /*_copy*/) = delete;
	void operator=(const TestLogicBuilder& /*_copy*/) = delete;

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class TestLogic : public UserComponent
{

	friend class TestLogicBuilder;

public:

private:

	TestLogic(Object* pObject);
	~TestLogic() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

};

JE_END