#include "MinerState.h"
#include "CustomLogicHeader.h"
#include "WifeState.h"

jeBegin

jeDefineCustomComponentBuilder(MinerState);
jeDefineCustomComponentBuilder(BeatBully);
jeDefineCustomComponentBuilder(EatStew);
jeDefineCustomComponentBuilder(GoHomeAndSleepTilRested);
jeDefineCustomComponentBuilder(QuenchThirst);
jeDefineCustomComponentBuilder(VisitBankAndDepositGold);
jeDefineCustomComponentBuilder(EnterMineAndDigForNugget);

/////////////////////////////////////////////////////////////////////////
// Global miner state
/////////////////////////////////////////////////////////////////////////
MinerState::MinerState(Object* _pObject)
    :CustomComponent(_pObject)
{}

void MinerState::Register()
{}

void MinerState::Load(CR_RJValue /*_data*/)
{}

void MinerState::Init()
{
    m_pTransform = m_pOwner->GetComponent<Transform>();

    // Add child object
    // Miner talks object
    FACTORY::CreateObject("MinerTalks");
    FACTORY::AddCreatedObject();
    m_minerTalks = FACTORY::GetCreatedObject();
    m_minerTalks->AddComponent<Transform>();
    m_minerTalks->AddComponent<Text>();
    m_talkTransform = m_minerTalks->GetComponent<Transform>();
    m_talkTransform->m_scale.Set(.15f, .15f, 0.f);
    m_talkOffset.Set(15.f, 10.f, 1.f);
    m_talkText = m_minerTalks->GetComponent<Text>();
    m_talkText->Register();
    m_pOwner->AddChild(m_minerTalks);

    // Set font
    m_talkText->m_pFont = ASSET::GetFont("Default");

}

void MinerState::Update(const float /*_dt*/)
{}

void MinerState::Close()
{}

bool MinerState::OnMessage(Telegram& /*msg*/)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////
// GoHomeAndSleepTilRested state
/////////////////////////////////////////////////////////////////////////
GoHomeAndSleepTilRested::GoHomeAndSleepTilRested(Object* _pObject)
    :CustomComponent(_pObject)
{}

void GoHomeAndSleepTilRested::Register()
{}

void GoHomeAndSleepTilRested::Load(CR_RJValue /*_data*/)
{}

void GoHomeAndSleepTilRested::Init()
{
	m_globalState = (MinerState*)m_pOwner->GetGlobalState();

	static bool firstTIme = true;

	if (!firstTIme) {
		if (!m_globalState->m_ateStew)
			DISPATCHER::DispatchMessage(0.0,			//time delay
				m_pOwnerId,								//sender ID
				CONTAINER->GetObject("Wife")->GetId(),	//receiver ID
				"HoneyI'mHome",							//msg
				nullptr);
	}

    m_globalState->m_location = HOME;

	m_globalState->m_pTransform->m_position.Set(-80.f, 0.f, 0.f);
	m_globalState->m_content = "Location: Home\nGetting sleep...";
	m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
		m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
		m_globalState->m_gold, m_globalState->m_saved);
	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);

	m_globalState->m_ateStew = firstTIme = false;
}

void GoHomeAndSleepTilRested::Update(const float /*_dt*/)
{
    if (m_globalState->m_fatigue <= 0) 
        m_pOwner->ChangeState<EnterMineAndDigForNugget>();

	m_globalState->m_fatigue--;
	if (m_globalState->m_fatigue < 0)
		m_globalState->m_fatigue = 0;

	m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
		m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
		m_globalState->m_gold, m_globalState->m_saved);
}

void GoHomeAndSleepTilRested::Close()
{}

bool GoHomeAndSleepTilRested::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "StewReady"))
	{
		m_pOwner->ChangeState<EatStew>();
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////
// EnterMineAndDigForNugget state
/////////////////////////////////////////////////////////////////////////
EnterMineAndDigForNugget::EnterMineAndDigForNugget(Object* _pObject)
    :CustomComponent(_pObject)
{}

void EnterMineAndDigForNugget::Register()
{}

void EnterMineAndDigForNugget::Load(CR_RJValue /*_data*/)
{}

void EnterMineAndDigForNugget::Init()
{
    m_globalState = (MinerState*)m_pOwner->GetGlobalState();
 
	if (m_globalState->m_gold >= 10)
		m_pOwner->ChangeState<VisitBankAndDepositGold>();

	// Check thirsty
	else if (m_globalState->m_thirst >= 10)
		m_pOwner->ChangeState<QuenchThirst>();

	// Check fatigue
	else if (m_globalState->m_fatigue >= 10)
		m_pOwner->ChangeState<GoHomeAndSleepTilRested>();
	
	else {
		m_globalState->m_location = GOLD_MINE;

		m_globalState->m_pTransform->m_position.Set(0.f, 0.f, 0.f);
		m_globalState->m_content = "Location: Mine\nDigging nugget...";
		m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
			m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
			m_globalState->m_gold, m_globalState->m_saved);
		m_globalState->m_talkTransform->m_position.Set(
			m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);
	}
}

void EnterMineAndDigForNugget::Update(const float /*_dt*/)
{
    if (m_globalState->m_gold >= 10)
        m_pOwner->ChangeState<VisitBankAndDepositGold>();

    // Check thirsty
    else if (m_globalState->m_thirst >= 10)
        m_pOwner->ChangeState<QuenchThirst>();

    // Check fatigue
    else if (m_globalState->m_fatigue >= 10)
        m_pOwner->ChangeState<GoHomeAndSleepTilRested>();

    else
    {
        m_globalState->m_gold++;
        m_globalState->m_fatigue++;
        m_globalState->m_thirst++;

        m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
            m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
            m_globalState->m_gold, m_globalState->m_saved);
    }
}

void EnterMineAndDigForNugget::Close()
{}

bool EnterMineAndDigForNugget::OnMessage(Telegram& /*msg*/)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////
// VisitBankAndDepositGold state
/////////////////////////////////////////////////////////////////////////
VisitBankAndDepositGold::VisitBankAndDepositGold(Object* _pObject)
    :CustomComponent(_pObject)
{}

void VisitBankAndDepositGold::Register()
{}

void VisitBankAndDepositGold::Load(CR_RJValue /*_data*/)
{}

void VisitBankAndDepositGold::Init()
{
    m_globalState = (MinerState*)m_pOwner->GetGlobalState();
    m_globalState->m_location = BANK;

	m_globalState->m_pTransform->m_position.Set(50.f, -50.f, 0.f);	
	m_globalState->m_saved += m_globalState->m_gold;
	m_globalState->m_gold = 0;

	m_globalState->m_content = "Location: Bank\nSave ma golds!";
	m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
		m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
		m_globalState->m_gold, m_globalState->m_saved);

	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);
}

void VisitBankAndDepositGold::Update(const float /*_dt*/)
{
    if (m_globalState->m_saved >= 100)
        m_pOwner->ChangeState<GoHomeAndSleepTilRested>();

    else
        m_pOwner->ChangeState<EnterMineAndDigForNugget>();

}

void VisitBankAndDepositGold::Close()
{}

bool VisitBankAndDepositGold::OnMessage(Telegram& /*msg*/)
{
    return false;
}

/////////////////////////////////////////////////////////////////////////
// QuenchThirst state
/////////////////////////////////////////////////////////////////////////
QuenchThirst::QuenchThirst(Object* _pObject)
    :CustomComponent(_pObject)
{}

void QuenchThirst::Register()
{}

void QuenchThirst::Load(CR_RJValue /*_data*/)
{}

void QuenchThirst::Init()
{
    m_globalState = (MinerState*)m_pOwner->GetGlobalState();
    m_globalState->m_location = PUB;

	m_globalState->m_pTransform->m_position.Set(-50.f, -50.f, 0.f);
	m_globalState->m_thirst = 0;

	m_globalState->m_content = "Location: Pub\nRum! Rum! Rum!";
	m_globalState->m_talkText->SetText("%s\nFatigue: %d\nThirst: %d\nGold: %d\nSaved: %d",
		m_globalState->m_content, m_globalState->m_fatigue, m_globalState->m_thirst,
		m_globalState->m_gold, m_globalState->m_saved);
	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);
}

void QuenchThirst::Update(const float /*_dt*/)
{
	if (m_globalState->m_thirst == 0)
		m_pOwner->ChangeState<EnterMineAndDigForNugget>();
}

void QuenchThirst::Close()
{}

bool QuenchThirst::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "Fight")) {
		m_pOwner->ChangeState<BeatBully>();
		return true;
	}

	return false;
}


/////////////////////////////////////////////////////////////////////////
// BeatBully state
/////////////////////////////////////////////////////////////////////////
BeatBully::BeatBully(Object* _pObject)
    :CustomComponent(_pObject)
{}

void BeatBully::Register()
{}

void BeatBully::Load(CR_RJValue /*_data*/)
{}

void BeatBully::Init()
{
    m_globalState = (MinerState*)m_pOwner->GetGlobalState();

	m_globalState->m_content = "You damn loser!\nYou cannot take me down!\nGet lost!";
	m_globalState->m_talkText->SetText("%s", m_globalState->m_content);
}

void BeatBully::Update(const float /*_dt*/)
{
	if (!m_beaten) 
		m_beaten = true;

	else {
		DISPATCHER::DispatchMessage(0.0,			//time delay
			m_pOwnerId,								//sender ID
			CONTAINER->GetObject("Bully")->GetId(),	//receiver ID
			"Fight",								//msg
			nullptr);
	}
}

void BeatBully::Close()
{}

bool BeatBully::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "Surrender")) {
		m_beaten = false;
		m_pOwner->RevertToPreviousState();
		return true;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////
// EatStew state
/////////////////////////////////////////////////////////////////////////
EatStew::EatStew(Object* _pObject)
    :CustomComponent(_pObject)
{}

void EatStew::Register()
{}

void EatStew::Load(CR_RJValue /*_data*/)
{}

void EatStew::Init()
{
    m_globalState = (MinerState*)m_pOwner->GetGlobalState();

	m_globalState->m_content = "Location: Home\nGod! What did you cook?\nA Tree?";
	m_globalState->m_talkText->SetText("%s", m_globalState->m_content);

	m_globalState->m_ateStew = true;
}

void EatStew::Update(const float /*_dt*/)
{
    m_pOwner->RevertToPreviousState();
}

void EatStew::Close()
{}

bool EatStew::OnMessage(Telegram& /*msg*/)
{
    return false;
}

jeEnd
