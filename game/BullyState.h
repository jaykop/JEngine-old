#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

JE_BEGIN

class Object;
struct Telegram;

JE_DECLARE_COMPONENT_BUILDER(BullyState)
JE_DECLARE_COMPONENT_BUILDER(JustHatingMiner)
JE_DECLARE_COMPONENT_BUILDER(GoFight)

/////////////////////////////////////////////////////////////////////////
// Global bully state
/////////////////////////////////////////////////////////////////////////
class BullyState : public CustomComponent
{
    friend class ComponentManager;
    friend class BullyStateBuilder;

public:

    Location	m_location = PUB;
    int		m_hateMiner = 0;
    int		m_minerId = 0;

private:

    BullyState(Object* pObject);
    ~BullyState() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Hating state
/////////////////////////////////////////////////////////////////////////
class JustHatingMiner : public CustomComponent
{
    friend class ComponentManager;
    friend class JustHatingMinerBuilder;

public:

private:

    JustHatingMiner(Object* pObject);
    ~JustHatingMiner() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};

};

/////////////////////////////////////////////////////////////////////////
// Go fight to miner
/////////////////////////////////////////////////////////////////////////
class GoFight : public CustomComponent
{
    friend class ComponentManager;
    friend class GoFightBuilder;

public:

    unsigned receiverId;

private:

    GoFight(Object* pObject);
    ~GoFight() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};

};

JE_END