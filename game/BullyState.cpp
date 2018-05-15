#include "BullyState.h"
#include "CustomLogicHeader.h"
#include "MinerState.h"

JE_BEGIN

JE_DEFINE_COMPONENT_BUILDER(BullyState)
JE_DEFINE_COMPONENT_BUILDER(JustHatingMiner)
JE_DEFINE_COMPONENT_BUILDER(GoFight)

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
JustHatingMiner::JustHatingMiner(Object* _pObject)
    :CustomComponent(_pObject)
{}

void JustHatingMiner::Register()
{}

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
GoFight::GoFight(Object* _pObject)
    :CustomComponent(_pObject)
{}

void GoFight::Register()
{}

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