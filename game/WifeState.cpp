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
{
	m_pTransform = m_pOwner->GetComponent<Transform>();

	// Add child object
	// Wife talks object
	FACTORY::CreateObject("WifeTalks");
	FACTORY::AddCreatedObject();
	m_wifeTalks = FACTORY::GetCreatedObject();
	m_wifeTalks->AddComponent<Transform>();
	m_wifeTalks->AddComponent<Text>();
	m_pTalkTransform = m_wifeTalks->GetComponent<Transform>();
	m_pTalkTransform->m_scale.Set(.125f, .125f, 0.f);
	m_talkOffset.Set(20.f, 0.f, 1.f);
	m_talkText = m_wifeTalks->GetComponent<Text>();
	m_talkText->Register();
	m_pOwner->AddChild(m_wifeTalks);

	// Set font
	m_talkText->m_pFont = ASSET::GetFont("Default");

	m_pTalkTransform->m_position.Set(
		m_pTransform->m_position + m_talkOffset);
}

void WifeState::Update(const float /*_dt*/)
{}

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
{
	m_globalState = (WifeState*)m_pOwner->GetGlobalState();

	m_globalState->m_content = "Doing houseworks...";
	m_globalState->m_talkText->SetText("%s\nChores: %d\nNeed to pee?: %d",
		m_globalState->m_content, m_globalState->m_chores, m_globalState->m_natureCalling);
}

void DoHousework::Update(const float /*_dt*/)
{
	m_globalState->m_natureCalling++;
	m_globalState->m_chores--;
	if (m_globalState->m_chores < 0)
		m_globalState->m_chores = 0;

	m_globalState->m_talkText->SetText("%s\nChores: %d\nNeed to pee?: %d",
		m_globalState->m_content, m_globalState->m_chores, m_globalState->m_natureCalling);

	if (m_globalState->m_natureCalling > 10)
		m_pOwner->ChangeState<GoToBathroom>();
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
	m_globalState = (WifeState*)m_pOwner->GetGlobalState();
	receiverId = CONTAINER->GetObject("Miner")->GetId();

    if (!m_globalState->m_isCooking)
    {
        //send a delayed message myself so that I know when to take the stew
        //out of the oven
        DISPATCHER::DispatchMessage(1.5,    //time delay
            m_pOwnerId,			    //sender ID
            receiverId,			    //receiver ID
            "StewReady",		    //msg
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
{
	m_globalState = (WifeState*)m_pOwner->GetGlobalState();

	m_globalState->m_natureCalling = 0;
	m_globalState->m_chores = 10;
	m_globalState->m_content = "Nature is calling me!!!!";
	m_globalState->m_talkText->SetText("%s\nChores: %d\nNeed to pee?: %d",
		m_globalState->m_content, m_globalState->m_chores, m_globalState->m_natureCalling);
}

void GoToBathroom::Update(const float /*_dt*/)
{
	if (!m_globalState->m_natureCalling)
		m_pOwner->ChangeState<DoHousework>();
}

void GoToBathroom::Close()
{}

bool GoToBathroom::OnMessage(Telegram& /*msg*/)
{
    return false;
}

JE_END
