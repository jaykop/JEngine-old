#include "StateMachine.h"
#include "CustomLogicHeader.h"

jeBegin

jeDefineCustomComponentBuilder(StateMachine);

StateMachine::StateMachine(Object* pObject)
    :CustomComponent(pObject)
{}

void StateMachine::Register()
{
    SYSTEM::pBehavior_->AddBehavior(this);
}

void StateMachine::Load(CR_RJValue _data)
{
    if (_data.HasMember("StateList"))
    {
        CR_RJValue stateList = _data["StateList"];
        for (rapidjson::SizeType i = 0; i < stateList.Size(); ++i)
            GetOwner()->AddComponent(stateList[i].GetString());
    }

    if (_data.HasMember("Current")) {
        CR_RJValue currentState = _data["Current"];
        GetOwner()->SetCurrentState(currentState.GetString());
    }

    if (_data.HasMember("Global")) {
        CR_RJValue globalState = _data["Global"];
        GetOwner()->SetGlobalState(globalState.GetString());
    }
}

void StateMachine::Init()
{
    if (GetOwner()->GetGlobalState())
        GetOwner()->GetGlobalState()->Init();

    if (GetOwner()->GetCurrentState())
        GetOwner()->GetCurrentState()->Init();
}

void StateMachine::Update(float dt)
{
    if (INPUT::KeyTriggered(JE_ENTER)) {
		
        if (GetOwner()->GetGlobalState())
            GetOwner()->GetGlobalState()->Update(dt);

        if (GetOwner()->GetCurrentState())
            GetOwner()->GetCurrentState()->Update(dt);
    }
}

void StateMachine::Close()
{}

void StateMachine::Unload()
{}

void StateMachine::EditorUpdate(const float /*dt*/)
{
    // TODO
}

jeEnd
