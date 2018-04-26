#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

JE_BEGIN

class Object;
struct Telegram;

/////////////////////////////////////////////////////////////////////////
// Global miner state
/////////////////////////////////////////////////////////////////////////
class MinerStateBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	MinerStateBuilder();
	~MinerStateBuilder() {};
	MinerStateBuilder(const MinerStateBuilder& /*_copy*/) = delete;
	void operator=(const MinerStateBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class MinerState : public CustomComponent
{
	friend class ComponentManager;
	friend class MinerStateBuilder;

public:

	Location	m_location;
	int			m_gold, m_money, m_thirst, m_fatigue, m_saved;

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
class BeatBullyBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	BeatBullyBuilder();
	~BeatBullyBuilder() {};
	BeatBullyBuilder(const BeatBullyBuilder& /*_copy*/) = delete;
	void operator=(const BeatBullyBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class BeatBully : public CustomComponent
{
	friend class ComponentManager;
	friend class BeatBullyBuilder;

public:

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
class EatStewBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	EatStewBuilder();
	~EatStewBuilder() {};
	EatStewBuilder(const EatStewBuilder& /*_copy*/) = delete;
	void operator=(const EatStewBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class EatStew : public CustomComponent
{
	friend class ComponentManager;
	friend class EatStewBuilder;

public:

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
class GoHomeAndSleepTilRestedBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	GoHomeAndSleepTilRestedBuilder();
	~GoHomeAndSleepTilRestedBuilder() {};
	GoHomeAndSleepTilRestedBuilder(const GoHomeAndSleepTilRestedBuilder& /*_copy*/) = delete;
	void operator=(const GoHomeAndSleepTilRestedBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class GoHomeAndSleepTilRested : public CustomComponent
{
	friend class ComponentManager;
	friend class GoHomeAndSleepTilRestedBuilder;

public:

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
class QuenchThirstBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	QuenchThirstBuilder();
	~QuenchThirstBuilder() {};
	QuenchThirstBuilder(const QuenchThirstBuilder& /*_copy*/) = delete;
	void operator=(const QuenchThirstBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class QuenchThirst : public CustomComponent
{
	friend class ComponentManager;
	friend class QuenchThirstBuilder;

public:

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
class VisitBankAndDepositGoldBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	VisitBankAndDepositGoldBuilder();
	~VisitBankAndDepositGoldBuilder() {};
	VisitBankAndDepositGoldBuilder(const VisitBankAndDepositGoldBuilder& /*_copy*/) = delete;
	void operator=(const VisitBankAndDepositGoldBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class VisitBankAndDepositGold : public CustomComponent
{
	friend class ComponentManager;
	friend class VisitBankAndDepositGoldBuilder;

public:

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
class EnterMineAndDigForNuggetBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	EnterMineAndDigForNuggetBuilder();
	~EnterMineAndDigForNuggetBuilder() {};
	EnterMineAndDigForNuggetBuilder(const EnterMineAndDigForNuggetBuilder& /*_copy*/) = delete;
	void operator=(const EnterMineAndDigForNuggetBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class EnterMineAndDigForNugget : public CustomComponent
{
	friend class ComponentManager;
	friend class EnterMineAndDigForNuggetBuilder;

public:

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