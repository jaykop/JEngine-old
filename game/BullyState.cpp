#include "BullyState.h"
#include "CustomLogicHeader.h"
#include "MinerState.h"

jeBegin

jeDefineCustomComponentBuilder(BullyState);
jeDefineCustomComponentBuilder(JustHatingMiner);
jeDefineCustomComponentBuilder(GoFight);

/////////////////////////////////////////////////////////////////////////
// Global bully state
/////////////////////////////////////////////////////////////////////////
BullyState::BullyState(Object* _pObject)
    :CustomComponent(_pObject)
{}

void BullyState::Register()
{}

void BullyState::Load(CR_RJValue /*_data*/)
{}

void BullyState::Init()
{
	m_pTransform = GetOwner()->GetComponent<Transform>();

	// Add child object
	// Wife talks object
	FACTORY::CreateObject("BullyTalks");
	FACTORY::AddCreatedObject();
	m_bullyTalks = FACTORY::GetCreatedObject();
	m_bullyTalks->AddComponent<Transform>();
	m_bullyTalks->AddComponent<Text>();
	m_pTalkTransform = m_bullyTalks->GetComponent<Transform>();
	m_pTalkTransform->scale.Set(.15f, .15f, 0.f);
	m_talkOffset.Set(-25.f, -20.f, 1.f);
	m_talkText = m_bullyTalks->GetComponent<Text>();
	m_talkText->Register();
	GetOwner()->AddChild(m_bullyTalks);

	// Set font
	m_talkText->pFont = ASSET::GetFont("Default");

	m_pTalkTransform->position.Set(
		m_pTransform->position + m_talkOffset);

	m_pMiner = CONTAINER->GetObject("Miner");
}

void BullyState::Update(const float /*_dt*/)
{}

void BullyState::Close()
{}

bool BullyState::OnMessage(Telegram& /*msg*/)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////
// Hating state
/////////////////////////////////////////////////////////////////////////
JustHatingMiner::JustHatingMiner(Object* _pObject)
    :CustomComponent(_pObject)
{}

void JustHatingMiner::Register()
{}

void JustHatingMiner::Load(CR_RJValue /*_data*/)
{}

void JustHatingMiner::Init()
{
	m_globalState = (BullyState*)GetOwner()->GetGlobalState();

	m_globalState->m_content = "I hate that miner!";
	m_globalState->m_talkText->SetText("%s\nWish to punch: %d",
		m_globalState->m_content, m_globalState->m_hateMiner);
}

void JustHatingMiner::Update(const float /*_dt*/)
{
	MinerState* pMinerState = (MinerState*)m_globalState->m_pMiner->GetGlobalState();
	if (m_globalState->m_hateMiner > 10
		&& pMinerState->m_location == PUB) {
		m_globalState->m_hateMiner = 0;
		GetOwner()->ChangeState<GoFight>();
	}

	else {
		m_globalState->m_hateMiner++;

		m_globalState->m_talkText->SetText("%s\nWish to punch: %d",
			m_globalState->m_content, m_globalState->m_hateMiner);
	}
}

void JustHatingMiner::Close()
{}

bool JustHatingMiner::OnMessage(Telegram& /*msg*/)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////
// Go fight to miner
/////////////////////////////////////////////////////////////////////////
GoFight::GoFight(Object* _pObject)
    :CustomComponent(_pObject)
{}

void GoFight::Register()
{}

void GoFight::Load(CR_RJValue /*_data*/)
{}

void GoFight::Init()
{
	m_globalState = (BullyState*)GetOwner()->GetGlobalState();

	DISPATCHER::DispatchMessage(0.0,			//time delay
		GetOwner()->GetId(),								//sender ID
		CONTAINER->GetObject("Miner")->GetId(),	//receiver ID
		"Fight",								//msg
		nullptr);

	m_globalState->m_content = "I hate you dirty miner!\nLet's take this outside!";
	m_globalState->m_talkText->SetText("%s", m_globalState->m_content);

}

void GoFight::Update(const float /*_dt*/)
{
	if (m_beaten) {
		m_beaten = false;
		GetOwner()->RevertToPreviousState();
	}
}

void GoFight::Close()
{}

bool GoFight::OnMessage(Telegram& msg)
{
    if (!strcmp(msg.pMessage, "Fight")) {

		m_globalState->m_content = "Shoot it hurts...\n";
		m_globalState->m_talkText->SetText("%s", m_globalState->m_content);

		DISPATCHER::DispatchMessage(0.0,			//time delay
			GetOwner()->GetId(),								//sender ID
			CONTAINER->GetObject("Miner")->GetId(),	//receiver ID
			"Surrender",								//msg
			nullptr);

		m_beaten = true;
        return true;
    }

    return false;
}

jeEnd
