#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

JE_BEGIN

class Object;
struct Telegram;

/////////////////////////////////////////////////////////////////////////
// Global bully state
/////////////////////////////////////////////////////////////////////////
class BullyStateBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	BullyStateBuilder();
	~BullyStateBuilder() {};
	BullyStateBuilder(const BullyStateBuilder& /*_copy*/) = delete;
	void operator=(const BullyStateBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class BullyState : public CustomComponent
{
	friend class ComponentManager;
	friend class BullyStateBuilder;

public:

	Location	m_location = PUB;
	int			m_hateMiner = 0;
	int			m_minerId = 0;

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

	void EditorUpdate(const float _dt) override {};

};

/////////////////////////////////////////////////////////////////////////
// Hating state
/////////////////////////////////////////////////////////////////////////
class JustHatingMinerBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	JustHatingMinerBuilder();
	~JustHatingMinerBuilder() {};
	JustHatingMinerBuilder(const JustHatingMinerBuilder& /*_copy*/) = delete;
	void operator=(const JustHatingMinerBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

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

	void EditorUpdate(const float _dt) override {};

};

/////////////////////////////////////////////////////////////////////////
// Go fight to miner
/////////////////////////////////////////////////////////////////////////
class GoFightBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	GoFightBuilder();
	~GoFightBuilder() {};
	GoFightBuilder(const GoFightBuilder& /*_copy*/) = delete;
	void operator=(const GoFightBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

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

	void EditorUpdate(const float _dt) override {};

};

JE_END