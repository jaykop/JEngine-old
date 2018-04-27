#include "StateMachine.h"
#include "GameLogicHeader.h"

JE_BEGIN

StateMachine::StateMachine(Object* _pObject)
	:CustomComponent(_pObject)
{}

void StateMachine::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void StateMachine::Load(CR_RJValue _data)
{
	if (_data.HasMember("Current")) {
		CR_RJValue currentState = _data["Current"];
		m_pOwner->SetCurrentState(currentState.GetString());
	}

	if (_data.HasMember("Global")) {
		CR_RJValue globalState = _data["Global"];
		m_pOwner->SetGlobalState(globalState.GetString());
	}
}

void StateMachine::Init()
{}

void StateMachine::Update(const float /*_dt*/)
{}

void StateMachine::Close()
{}

void StateMachine::Unload()
{}

void StateMachine::EditorUpdate(const float /*_dt*/)
{
	// TODO
}

StateMachineBuilder::StateMachineBuilder()
	:ComponentBuilder()
{}

CustomComponent* StateMachineBuilder::CreateComponent(Object* _pOwner) const
{
	return new StateMachine(_pOwner);
}

JE_END