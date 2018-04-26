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

void MinerState::Load(CR_RJValue _data)
{}

void MinerState::Init()
{
}

void MinerState::Update(const float _dt)
{
}

void MinerState::Close()
{
}

bool MinerState::OnMessage(Telegram& msg)
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

void BeatBully::Load(CR_RJValue _data)
{}

void BeatBully::Init()
{
}

void BeatBully::Update(const float _dt)
{
}

void BeatBully::Close()
{
}

bool BeatBully::OnMessage(Telegram& msg)
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

void EatStew::Load(CR_RJValue _data)
{}

void EatStew::Init()
{
}

void EatStew::Update(const float _dt)
{
}

void EatStew::Close()
{
}

bool EatStew::OnMessage(Telegram& msg)
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

void GoHomeAndSleepTilRested::Load(CR_RJValue _data)
{}

void GoHomeAndSleepTilRested::Init()
{
}

void GoHomeAndSleepTilRested::Update(const float _dt)
{
}

void GoHomeAndSleepTilRested::Close()
{
}

bool GoHomeAndSleepTilRested::OnMessage(Telegram& msg)
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

void QuenchThirst::Load(CR_RJValue _data)
{}

void QuenchThirst::Init()
{
}

void QuenchThirst::Update(const float _dt)
{
}

void QuenchThirst::Close()
{
}

bool QuenchThirst::OnMessage(Telegram& msg)
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

void VisitBankAndDepositGold::Load(CR_RJValue _data)
{}

void VisitBankAndDepositGold::Init()
{
}

void VisitBankAndDepositGold::Update(const float _dt)
{
}

void VisitBankAndDepositGold::Close()
{
}

bool VisitBankAndDepositGold::OnMessage(Telegram& msg)
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

void EnterMineAndDigForNugget::Load(CR_RJValue _data)
{}

void EnterMineAndDigForNugget::Init()
{
}

void EnterMineAndDigForNugget::Update(const float _dt)
{
}

void EnterMineAndDigForNugget::Close()
{
}

bool EnterMineAndDigForNugget::OnMessage(Telegram& msg)
{
	return false;
}

JE_END
