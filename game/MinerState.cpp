#include "MinerState.h"
#include "GameLogicHeader.h"

JE_BEGIN

/////////////////////////////////////////////////////////////////////////
// Global miner state
/////////////////////////////////////////////////////////////////////////
MinerStateBuilder::MinerStateBuilder()
	:ComponentBuilder()
{}

CustomComponent* MinerStateBuilder::CreateComponent(Object* _pOwner) const
{
	return new MinerState(_pOwner);
}

MinerState::MinerState(Object* _pObject)
	:CustomComponent(_pObject)
{}

void MinerState::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void MinerState::Load(CR_RJValue /*_data*/)
{}

void MinerState::Init()
{
	m_text = m_pOwner->GetComponent<Text>();
}

void MinerState::Update(const float /*_dt*/)
{	
	// Check thirsty
	if (m_thirst >= 10)
		m_pOwner->ChangeState<QuenchThirst>();

	// Check fatigue
	if (m_fatigue >= 10)
		m_pOwner->ChangeState<GoHomeAndSleepTilRested>();
	
	switch (m_location)
	{
	case HOME: 
	}

	m_text->SetText("Location: %s", m_content);
}

void MinerState::Close()
{
}

bool MinerState::OnMessage(Telegram& /*msg*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////
// BeatBully state
/////////////////////////////////////////////////////////////////////////
BeatBullyBuilder::BeatBullyBuilder()
	:ComponentBuilder()
{}

CustomComponent* BeatBullyBuilder::CreateComponent(Object* _pOwner) const
{
	return new BeatBully(_pOwner);
}

BeatBully::BeatBully(Object* _pObject)
	:CustomComponent(_pObject)
{}

void BeatBully::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void BeatBully::Load(CR_RJValue /*_data*/)
{}

void BeatBully::Init()
{}

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
EatStewBuilder::EatStewBuilder()
	:ComponentBuilder()
{}

CustomComponent* EatStewBuilder::CreateComponent(Object* _pOwner) const
{
	return new EatStew(_pOwner);
}

EatStew::EatStew(Object* _pObject)
	:CustomComponent(_pObject)
{}

void EatStew::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void EatStew::Load(CR_RJValue /*_data*/)
{}

void EatStew::Init()
{}

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
// GoHomeAndSleepTilRested state
/////////////////////////////////////////////////////////////////////////
GoHomeAndSleepTilRestedBuilder::GoHomeAndSleepTilRestedBuilder()
	:ComponentBuilder()
{}

CustomComponent* GoHomeAndSleepTilRestedBuilder::CreateComponent(Object* _pOwner) const
{
	return new GoHomeAndSleepTilRested(_pOwner);
}

GoHomeAndSleepTilRested::GoHomeAndSleepTilRested(Object* _pObject)
	:CustomComponent(_pObject)
{}

void GoHomeAndSleepTilRested::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void GoHomeAndSleepTilRested::Load(CR_RJValue /*_data*/)
{}

void GoHomeAndSleepTilRested::Init()
{
	m_pOwner->GetComponent<MinerState>()->m_location = HOME;
}

void GoHomeAndSleepTilRested::Update(const float /*_dt*/)
{
	m_pOwner->GetComponent<MinerState>()->m_fatigue--;

	if (m_pOwner->GetComponent<MinerState>()->m_fatigue <= 0)
		m_pOwner->ChangeState<EnterMineAndDigForNugget>();
}

void GoHomeAndSleepTilRested::Close()
{}

bool GoHomeAndSleepTilRested::OnMessage(Telegram& /*msg*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////
// QuenchThirst state
/////////////////////////////////////////////////////////////////////////
QuenchThirstBuilder::QuenchThirstBuilder()
	:ComponentBuilder()
{}

CustomComponent* QuenchThirstBuilder::CreateComponent(Object* _pOwner) const
{
	return new QuenchThirst(_pOwner);
}

QuenchThirst::QuenchThirst(Object* _pObject)
	:CustomComponent(_pObject)
{}

void QuenchThirst::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void QuenchThirst::Load(CR_RJValue /*_data*/)
{}

void QuenchThirst::Init()
{
	m_pOwner->GetComponent<MinerState>()->m_location = RIVER;
}

void QuenchThirst::Update(const float /*_dt*/)
{
	m_pOwner->GetComponent<MinerState>()->m_thirst = 0;
	m_pOwner->ChangeState<EnterMineAndDigForNugget>();
}

void QuenchThirst::Close()
{}

bool QuenchThirst::OnMessage(Telegram& /*msg*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////
// VisitBankAndDepositGold state
/////////////////////////////////////////////////////////////////////////
VisitBankAndDepositGoldBuilder::VisitBankAndDepositGoldBuilder()
	:ComponentBuilder()
{}

CustomComponent* VisitBankAndDepositGoldBuilder::CreateComponent(Object* _pOwner) const
{
	return new VisitBankAndDepositGold(_pOwner);
}

VisitBankAndDepositGold::VisitBankAndDepositGold(Object* _pObject)
	:CustomComponent(_pObject)
{}

void VisitBankAndDepositGold::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void VisitBankAndDepositGold::Load(CR_RJValue /*_data*/)
{}

void VisitBankAndDepositGold::Init()
{
	m_pOwner->GetComponent<MinerState>()->m_location = BANK;
}

void VisitBankAndDepositGold::Update(const float /*_dt*/)
{
	m_pOwner->GetComponent<MinerState>()->m_saved 
		= m_pOwner->GetComponent<MinerState>()->m_gold;
	m_pOwner->GetComponent<MinerState>()->m_gold = 0;

	if (m_pOwner->GetComponent<MinerState>()->m_saved >= 100)
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
// EnterMineAndDigForNugget state
/////////////////////////////////////////////////////////////////////////
EnterMineAndDigForNuggetBuilder::EnterMineAndDigForNuggetBuilder()
	:ComponentBuilder()
{}

CustomComponent* EnterMineAndDigForNuggetBuilder::CreateComponent(Object* _pOwner) const
{
	return new EnterMineAndDigForNugget(_pOwner);
}

EnterMineAndDigForNugget::EnterMineAndDigForNugget(Object* _pObject)
	:CustomComponent(_pObject)
{}

void EnterMineAndDigForNugget::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void EnterMineAndDigForNugget::Load(CR_RJValue /*_data*/)
{}

void EnterMineAndDigForNugget::Init()
{
	m_pOwner->GetComponent<MinerState>()->m_location = GOLD_MINE;
}

void EnterMineAndDigForNugget::Update(const float /*_dt*/)
{
	MinerState* pGlobalState = m_pOwner->GetComponent<MinerState>();
	pGlobalState->m_gold++;
	pGlobalState->m_fatigue++;
	pGlobalState->m_thirst++;

	if (pGlobalState->m_gold >= 10)
		m_pOwner->ChangeState<VisitBankAndDepositGold>();
}

void EnterMineAndDigForNugget::Close()
{}

bool EnterMineAndDigForNugget::OnMessage(Telegram& /*msg*/)
{
	return false;
}

JE_END
