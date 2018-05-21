#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"

jeBegin

class Object;
struct Telegram;

class StateMachineBuilder : public ComponentBuilder
{

    friend class JEngine;

public:

private:

    StateMachineBuilder();
    ~StateMachineBuilder() {};
    StateMachineBuilder(const StateMachineBuilder& /*_copy*/) = delete;
    void operator=(const StateMachineBuilder& /*_copy*/) = delete;

    CustomComponent* CreateComponent(Object* _pOwner) const override;

};

void threadTest(int a);
class StateMachine : public CustomComponent
{
    friend class ComponentManager;
    friend class StateMachineBuilder;

public:

private:

    StateMachine(Object* pObject);
    ~StateMachine() {};

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
