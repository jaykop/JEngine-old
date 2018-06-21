#pragma once
#include "CustomComponent.h"
#include "ComponentManager.h"
#include "ComponentBuilder.h"

#include "Location.h"
#include "Vector3.h"

jeBegin

class Text;
class Object;
class Transform;
struct Telegram;

/////////////////////////////////////////////////////////////////////////
// Global miner state
/////////////////////////////////////////////////////////////////////////
class MinerState : public CustomComponent
{
    jeBaseFriends(MinerState);

public:

    Location	m_location;
    bool	m_ateStew = false;
    int		m_gold = 0, m_thirst = 0, m_fatigue = 0, m_saved = 0;
    const char	*m_content = nullptr;
    Object	*m_minerTalks = nullptr;
    Text	*m_talkText = nullptr;
    Transform	*m_pTransform = nullptr, *m_talkTransform = nullptr;
    vec3	m_talkOffset;

private:

    MinerState(Object* pObject);
    ~MinerState() {};

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
// BeatBully state
/////////////////////////////////////////////////////////////////////////
class BeatBully : public CustomComponent
{
    jeBaseFriends(BeatBully);

public:

    bool	m_beaten = false;
    unsigned	m_receiverId;
    MinerState	*m_globalState = nullptr;

private:

    BeatBully(Object* pObject);
    ~BeatBully() {};

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
// EatStew state
/////////////////////////////////////////////////////////////////////////
class EatStew : public CustomComponent
{
    jeBaseFriends(EatStew);

public:

    MinerState *m_globalState = nullptr;

private:

    EatStew(Object* pObject);
    ~EatStew() {};

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
// GoHomeAndSleepTilRested state
/////////////////////////////////////////////////////////////////////////
class GoHomeAndSleepTilRested : public CustomComponent
{
    jeBaseFriends(GoHomeAndSleepTilRested);

public:

    MinerState *m_globalState = nullptr;

private:

    GoHomeAndSleepTilRested(Object* pObject);
    ~GoHomeAndSleepTilRested() {};

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
// QuenchThirst state
/////////////////////////////////////////////////////////////////////////
class QuenchThirst : public CustomComponent
{
    jeBaseFriends(QuenchThirst);

public:

    MinerState *m_globalState = nullptr;

private:

    QuenchThirst(Object* pObject);
    ~QuenchThirst() {};

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
// VisitBankAndDepositGold state
/////////////////////////////////////////////////////////////////////////
class VisitBankAndDepositGold : public CustomComponent
{
    jeBaseFriends(VisitBankAndDepositGold);

public:

    MinerState *m_globalState = nullptr;

private:

    VisitBankAndDepositGold(Object* pObject);
    ~VisitBankAndDepositGold() {};

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
// EnterMineAndDigForNugget state
/////////////////////////////////////////////////////////////////////////
class EnterMineAndDigForNugget : public CustomComponent
{
	template <class T>
	friend class MemoryAllocator;

    friend class ComponentManager;
    friend class EnterMineAndDigForNuggetBuilder;

public:

    MinerState *m_globalState = nullptr;

private:

    EnterMineAndDigForNugget(Object* pObject);
    ~EnterMineAndDigForNugget() {};

    void Register() override;
    void Load(CR_RJValue _data) override;
    void Init() override;
    void Update(const float _dt) override;
    void Close() override;
    void Unload() override {};
    bool OnMessage(Telegram& msg) override;

    void EditorUpdate(const float /*_dt*/) override {};
};

jeDeclareCustomComponentBuilder(MinerState);
jeDeclareCustomComponentBuilder(BeatBully);
jeDeclareCustomComponentBuilder(EatStew);
jeDeclareCustomComponentBuilder(GoHomeAndSleepTilRested);
jeDeclareCustomComponentBuilder(QuenchThirst);
jeDeclareCustomComponentBuilder(VisitBankAndDepositGold);
jeDeclareCustomComponentBuilder(EnterMineAndDigForNugget);

jeEnd
