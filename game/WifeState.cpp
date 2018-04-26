#include "WifeState.h"
#include "GameLogicHeader.h"

JE_BEGIN

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
WifeStateBuilder::WifeStateBuilder()
	:ComponentBuilder()
{}

CustomComponent* WifeStateBuilder::CreateComponent(Object* _pOwner) const
{
	return new WifeState(_pOwner);
}

WifeState::WifeState(Object* _pObject)
	:CustomComponent(_pObject)
{}

void WifeState::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void WifeState::Load(CR_RJValue _data)
{}

void WifeState::Init()
{}

void WifeState::Update(const float _dt)
{
	if (m_natureCalling > 10)
		m_pOwner->ChangeState<GoToBathroom>();
}

void WifeState::Close()
{}

bool WifeState::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "HoneyImHome")) {
	
		//TODO 
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////
// Housework state
/////////////////////////////////////////////////////////////////////////
DoHouseworkBuilder::DoHouseworkBuilder()
	:ComponentBuilder()
{}

CustomComponent* DoHouseworkBuilder::CreateComponent(Object* _pOwner) const
{
	return new DoHousework(_pOwner);
}

DoHousework::DoHousework(Object* _pObject)
	:CustomComponent(_pObject)
{}

void DoHousework::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void DoHousework::Load(CR_RJValue /*_data*/)
{}

void DoHousework::Init()
{}

void DoHousework::Update(const float /*_dt*/)
{
	m_pOwner->GetComponent<WifeState>()->m_natureCalling++;
}

void DoHousework::Close()
{}

bool DoHousework::OnMessage(Telegram& /*msg*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////
// Cook Stew state
/////////////////////////////////////////////////////////////////////////
CookStewBuilder::CookStewBuilder()
	:ComponentBuilder()
{}

CustomComponent* CookStewBuilder::CreateComponent(Object* _pOwner) const
{
	return new CookStew(_pOwner);
}

CookStew::CookStew(Object* _pObject)
	:CustomComponent(_pObject)
{}

void CookStew::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void CookStew::Load(CR_RJValue /*_data*/)
{}

void CookStew::Init()
{
	if (!m_pOwner->GetComponent<WifeState>()->m_isCooking)
	{

		//send a delayed message myself so that I know when to take the stew
		//out of the oven
		DISPATCHER::DispatchMessage(1.5,	//time delay
			m_pOwnerId,						//sender ID
			receiverId,						//receiver ID
			"StewReady",					//msg
			nullptr);

		m_pOwner->GetComponent<WifeState>()->m_isCooking = true;
	}
}

void CookStew::Update(const float /*_dt*/)
{}

void CookStew::Close()
{}

bool CookStew::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "StewReady"))
	{
		//TODO
		//let hubby know the stew is ready
		DISPATCHER::DispatchMessage(0.0,
			m_pOwnerId,
			receiverId,
			"StewReady",
			nullptr);

		m_pOwner->GetComponent<WifeState>()->m_isCooking = false;
		m_pOwner->ChangeState<DoHousework>();

		return true;

	}

	return false;
}

/////////////////////////////////////////////////////////////////////////
// GoToBathroom state
/////////////////////////////////////////////////////////////////////////
GoToBathroomBuilder::GoToBathroomBuilder()
	:ComponentBuilder()
{}

CustomComponent* GoToBathroomBuilder::CreateComponent(Object* _pOwner) const
{
	return new GoToBathroom(_pOwner);
}

GoToBathroom::GoToBathroom(Object* _pObject)
	:CustomComponent(_pObject)
{}

void GoToBathroom::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void GoToBathroom::Load(CR_RJValue /*_data*/)
{}

void GoToBathroom::Init()
{}

void GoToBathroom::Update(const float _dt)
{
	m_pOwner->GetComponent<WifeState>()->m_natureCalling = 0;
	m_pOwner->ChangeState<DoHousework>();
}

void GoToBathroom::Close()
{}

bool GoToBathroom::OnMessage(Telegram& /*msg*/)
{
	return false;
}

JE_END
