#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

#include "Vector3.h"

JE_BEGIN

class Text;
class Object;
class Transform;
struct Telegram;

JE_DECLARE_COMPONENT_BUILDER(MinerState)
JE_DECLARE_COMPONENT_BUILDER(BeatBully)
JE_DECLARE_COMPONENT_BUILDER(EatStew)
JE_DECLARE_COMPONENT_BUILDER(GoHomeAndSleepTilRested)
JE_DECLARE_COMPONENT_BUILDER(QuenchThirst)
JE_DECLARE_COMPONENT_BUILDER(VisitBankAndDepositGold)
JE_DECLARE_COMPONENT_BUILDER(EnterMineAndDigForNugget)

/////////////////////////////////////////////////////////////////////////
// Global miner state
/////////////////////////////////////////////////////////////////////////
class MinerState : public CustomComponent
{
	friend class ComponentManager;
	friend class MinerStateBuilder;

public:

	Location	m_location;
	int			m_gold, m_money, m_thirst, m_fatigue, m_saved;
	const char	*m_content = nullptr;
	Object		*m_minerTalks = nullptr, *m_minerInfo = nullptr;
	Text		*m_talkText = nullptr, *m_infoText = nullptr;
	Transform	*m_pTransform = nullptr, *m_talkTransform = nullptr;
	vec3		m_talkOffset{ 0.f, 0.f, 1.f };

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
	friend class ComponentManager;
	friend class BeatBullyBuilder;

public:

	unsigned m_receiverId;
	MinerState * m_globalState = nullptr;

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
	friend class ComponentManager;
	friend class EatStewBuilder;

public:

	MinerState * m_globalState = nullptr;

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
	friend class ComponentManager;
	friend class GoHomeAndSleepTilRestedBuilder;

public:

	MinerState * m_globalState = nullptr;

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
	friend class ComponentManager;
	friend class QuenchThirstBuilder;

public:

	MinerState * m_globalState = nullptr;

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
	friend class ComponentManager;
	friend class VisitBankAndDepositGoldBuilder;

public:

	MinerState * m_globalState = nullptr;

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
	friend class ComponentManager;
	friend class EnterMineAndDigForNuggetBuilder;

public:

	MinerState * m_globalState = nullptr;

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

JE_END