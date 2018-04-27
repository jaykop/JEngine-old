#include "BullyState.h"
#include "GameLogicHeader.h"
#include "MinerState.h"

JE_BEGIN

/////////////////////////////////////////////////////////////////////////
// Global bully state
/////////////////////////////////////////////////////////////////////////
BullyStateBuilder::BullyStateBuilder()
	:ComponentBuilder()
{}

CustomComponent* BullyStateBuilder::CreateComponent(Object* _pOwner) const
{
	return new BullyState(_pOwner);
}

BullyState::BullyState(Object* _pObject)
	:CustomComponent(_pObject)
{}

void BullyState::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void BullyState::Load(CR_RJValue /*_data*/)
{}

void BullyState::Init()
{}

void BullyState::Update(const float /*_dt*/)
{
	Location whereIsMiner = 
		CONTAINER->GetObject(m_minerId)->GetComponent<MinerState>()->m_location;

	if (m_hateMiner >= 5 && whereIsMiner) 
		m_pOwner->ChangeState<GoFight>();
}

void BullyState::Close()
{}

bool BullyState::OnMessage(Telegram& /*msg*/)
{
	return false;
}

/////////////////////////////////////////////////////////////////////////
// Hating state
/////////////////////////////////////////////////////////////////////////
JustHatingMinerBuilder::JustHatingMinerBuilder()
	:ComponentBuilder()
{}

CustomComponent* JustHatingMinerBuilder::CreateComponent(Object* _pOwner) const
{
	return new JustHatingMiner(_pOwner);
}

JustHatingMiner::JustHatingMiner(Object* _pObject)
	:CustomComponent(_pObject)
{}

void JustHatingMiner::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void JustHatingMiner::Load(CR_RJValue /*_data*/)
{}

void JustHatingMiner::Init()
{}

void JustHatingMiner::Update(const float /*_dt*/)
{
	// TODO: Hate miner
	m_pOwner->GetComponent<BullyState>()->m_hateMiner++;
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
GoFightBuilder::GoFightBuilder()
	:ComponentBuilder()
{}

CustomComponent* GoFightBuilder::CreateComponent(Object* _pOwner) const
{
	return new GoFight(_pOwner);
}

GoFight::GoFight(Object* _pObject)
	:CustomComponent(_pObject)
{}

void GoFight::Register()
{
	SYSTEM::GetBehaviorSystem()->AddBehavior(this);
}

void GoFight::Load(CR_RJValue /*_data*/)
{}

void GoFight::Init()
{	
	// TODO: INIT FIGHT
	DISPATCHER::DispatchMessage(0.0,	//time delay
		m_pOwnerId,						//sender ID
		receiverId,						//receiver ID
		"Fight",						//msg
		nullptr);
}

void GoFight::Update(const float /*_dt*/)
{
	// TODO: EXCEUTE FIGHT
	m_pOwner->GetComponent<BullyState>()->m_hateMiner = 0;
}

void GoFight::Close()
{}

bool GoFight::OnMessage(Telegram& msg)
{
	if (!strcmp(msg.message, "Fight")) {
		m_pOwner->RevertToPreviousState();
		return true;
	}

	return false;
}

JE_END