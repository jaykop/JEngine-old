#pragma once
#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

jeBegin

class Object;
struct Telegram;

class StateMachine : public CustomComponent
{
	template <class T>
	friend class MemoryAllocator;

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

jeDeclareCustomComponentBuilder(StateMachine);
jeEnd
