#include "WifeState.h"
#include "CustomLogicHeader.h"

JE_BEGIN

JE_DEFINE_COMPONENT_BUILDER(WifeState)
JE_DEFINE_COMPONENT_BUILDER(DoHousework)
JE_DEFINE_COMPONENT_BUILDER(CookStew)
JE_DEFINE_COMPONENT_BUILDER(GoToBathroom)

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
WifeState::WifeState(Object* _pObject)
	:CustomComponent(_pObject)
{}

void WifeState::Register()
{}

void WifeState::Load(CR_RJValue /*_data*/)
{}

void WifeState::Init()
{}

void WifeState::Update(const float /*_dt*/)
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
DoHousework::DoHousework(Object* _pObject)
	:CustomComponent(_pObject)
{}

void DoHousework::Register()
{}

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
CookStew::CookStew(Object* _pObject)
	:CustomComponent(_pObject)
{}

void CookStew::Register()
{}

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
GoToBathroom::GoToBathroom(Object* _pObject)
	:CustomComponent(_pObject)
{}

void GoToBathroom::Register()
{}

void GoToBathroom::Load(CR_RJValue /*_data*/)
{}

void GoToBathroom::Init()
{}

void GoToBathroom::Update(const float /*_dt*/)
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
