#include "MinerState.h"
#include "CustomLogicHeader.h"

JE_BEGIN

JE_DEFINE_COMPONENT_BUILDER(MinerState)
JE_DEFINE_COMPONENT_BUILDER(BeatBully)
JE_DEFINE_COMPONENT_BUILDER(EatStew)
JE_DEFINE_COMPONENT_BUILDER(GoHomeAndSleepTilRested)
JE_DEFINE_COMPONENT_BUILDER(QuenchThirst)
JE_DEFINE_COMPONENT_BUILDER(VisitBankAndDepositGold)
JE_DEFINE_COMPONENT_BUILDER(EnterMineAndDigForNugget)

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
	m_talkOffset.Set( -30.f, 40.f, 1.f);
	m_pTransform = m_pOwner->GetComponent<Transform>();

	// Add child object
	// Miner talks object
	FACTORY::CreateObject("MinerTalks");
	FACTORY::AddCreatedObject();
	m_minerTalks= FACTORY::GetCreatedObject();
	m_minerTalks->AddComponent<Transform>();
	m_minerTalks->AddComponent<Text>();
	m_talkTransform = m_minerTalks->GetComponent<Transform>();
	m_talkTransform->m_scale.Set(.25f, .25f, 0.f);
	m_talkText = m_minerTalks->GetComponent<Text>();
	m_talkText->Register();
	m_pOwner->AddChild(m_minerTalks);

	// Miner info object
	FACTORY::CreateObject("MinerInfo");
	FACTORY::AddCreatedObject();
	m_minerInfo = FACTORY::GetCreatedObject();
	m_minerInfo->AddComponent<Transform>();
	m_minerInfo->AddComponent<Text>();
	m_infoText = m_minerTalks->GetComponent<Text>();
	m_infoText->Register();
	m_pOwner->AddChild(m_minerInfo);

	// Set font
	m_talkText->m_pFont = m_infoText->m_pFont = ASSET::GetFont("JKDot");
}

void MinerState::Update(const float /*_dt*/)
{
	// Check thirsty
	if (m_thirst >= 10)
		m_pOwner->ChangeState<QuenchThirst>();

	// Check fatigue
	else if (m_fatigue >= 10)
		m_pOwner->ChangeState<GoHomeAndSleepTilRested>();

	m_talkTransform->m_position.Set(
		m_pTransform->m_position + m_talkOffset);
}

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
	m_globalState->m_location = HOME;
	m_globalState->m_content = "Location: Home\nGetting sleep...";
	m_globalState->m_pTransform->m_position.Set(-80.f, 0.f, 0.f);
	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset); 
	m_globalState->m_talkText->SetText("%s\nFatigue: %d", m_globalState->m_content, m_globalState->m_fatigue);
}

void GoHomeAndSleepTilRested::Update(const float /*_dt*/)
{
	m_globalState->m_fatigue--;
	m_globalState->m_talkText->SetText("%s\nFatigue: %d", m_globalState->m_content, m_globalState->m_fatigue);

	if (m_globalState->m_fatigue <= 0) {
		m_globalState->m_fatigue = 0;
		m_pOwner->ChangeState<EnterMineAndDigForNugget>();
	}
}

void GoHomeAndSleepTilRested::Close()
{}

bool GoHomeAndSleepTilRested::OnMessage(Telegram& /*msg*/)
{
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
	m_globalState->m_location = GOLD_MINE;
	m_globalState->m_pTransform->m_position.Set(0.f, 0.f, 0.f);
	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);
	m_globalState->m_content = "Location: Mine\nDigging nugget...";
	m_globalState->m_talkText->SetText("%s\nGold: %d", m_globalState->m_content, m_globalState->m_gold);
}

void EnterMineAndDigForNugget::Update(const float /*_dt*/)
{
	m_globalState->m_gold++;
	m_globalState->m_fatigue++;
	m_globalState->m_thirst++;
	m_globalState->m_talkText->SetText("%s\nGold: %d", m_globalState->m_content, m_globalState->m_gold);

	if (m_globalState->m_gold >= 10)
		m_pOwner->ChangeState<VisitBankAndDepositGold>();
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
	m_globalState->m_talkTransform->m_position.Set(
		m_globalState->m_pTransform->m_position + m_globalState->m_talkOffset);
	m_globalState->m_content = "Location: Bank\nSave ma golds!";
	m_globalState->m_saved += m_globalState->m_gold;
	m_globalState->m_gold = 0;
	m_globalState->m_talkText->SetText("%s\nSaved: %d", m_globalState->m_content, m_globalState->m_saved);
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
}

void BeatBully::Update(const float /*_dt*/)
{
	DISPATCHER::DispatchMessage(0.0,	//time delay
		m_pOwnerId,						//sender ID
		m_receiverId,					//receiver ID
		"Fight",						//msg
		nullptr);

	m_pOwner->RevertToPreviousState();
}

void BeatBully::Close()
{}

bool BeatBully::OnMessage(Telegram& /*msg*/)
{
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
	m_globalState->m_location = RIVER;
}

void QuenchThirst::Update(const float /*_dt*/)
{
	m_globalState->m_thirst = 0;
	m_pOwner->ChangeState<EnterMineAndDigForNugget>();
}

void QuenchThirst::Close()
{}

bool QuenchThirst::OnMessage(Telegram& /*msg*/)
{
	return false;
}

JE_END
