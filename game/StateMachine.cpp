#include "StateMachine.h"
#include "CustomLogicHeader.h"

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
	if (_data.HasMember("StateList"))
	{
		CR_RJValue stateList = _data["StateList"];
		for (rapidjson::SizeType i = 0; i < stateList.Size(); ++i) 
			m_pOwner->AddComponent(stateList[i].GetString());
	}

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
{
	if (m_pOwner->GetGlobalState())
		m_pOwner->GetGlobalState()->Init();

	if (m_pOwner->GetCurrentState())
		m_pOwner->GetCurrentState()->Init();
}

void StateMachine::Update(const float _dt)
{
	if (INPUT::KeyTriggered(JE_ENTER)) {

		if (m_pOwner->GetGlobalState())
			m_pOwner->GetGlobalState()->Update(_dt);

		if (m_pOwner->GetCurrentState())
			m_pOwner->GetCurrentState()->Update(_dt);
	}
}

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