#pragma once
#include "CustomComponent.h"
#include "ComponentBuilder.h"
#include "Location.h"

JE_BEGIN

class Object;
struct Telegram;

/////////////////////////////////////////////////////////////////////////
// Global wife state
/////////////////////////////////////////////////////////////////////////
class WifeStateBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	WifeStateBuilder();
	~WifeStateBuilder() {};
	WifeStateBuilder(const WifeStateBuilder& /*_copy*/) = delete;
	void operator=(const WifeStateBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class WifeState : public CustomComponent
{
	friend class ComponentManager;
	friend class WifeStateBuilder;

public:

	Location	m_location;
	int			m_natureCalling, m_chores;
	int			m_minerId = 0;
	bool		m_isCooking = false;

private:

	WifeState(Object* pObject);
	~WifeState() {};

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
// Housework state
/////////////////////////////////////////////////////////////////////////
class DoHouseworkBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	DoHouseworkBuilder();
	~DoHouseworkBuilder() {};
	DoHouseworkBuilder(const DoHouseworkBuilder& /*_copy*/) = delete;
	void operator=(const DoHouseworkBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class DoHousework : public CustomComponent
{
	friend class ComponentManager;
	friend class DoHouseworkBuilder;

public:

private:

	DoHousework(Object* pObject);
	~DoHousework() {};

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
// Cook Stew state
/////////////////////////////////////////////////////////////////////////
class CookStewBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	CookStewBuilder();
	~CookStewBuilder() {};
	CookStewBuilder(const CookStewBuilder& /*_copy*/) = delete;
	void operator=(const CookStewBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class CookStew : public CustomComponent
{
	friend class ComponentManager;
	friend class CookStewBuilder;

public:

	unsigned receiverId;

private:

	CookStew(Object* pObject);
	~CookStew() {};

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
// GoToBathroom state
/////////////////////////////////////////////////////////////////////////
class GoToBathroomBuilder : public ComponentBuilder
{

	friend class JEngine;

public:

private:

	GoToBathroomBuilder();
	~GoToBathroomBuilder() {};
	GoToBathroomBuilder(const GoToBathroomBuilder& /*_copy*/) = delete;
	void operator=(const GoToBathroomBuilder& /*_copy*/) = delete;

	CustomComponent* CreateComponent(Object* _pOwner) const override;

};

class GoToBathroom : public CustomComponent
{
	friend class ComponentManager;
	friend class GoToBathroomBuilder;

public:

private:

	GoToBathroom(Object* pObject);
	~GoToBathroom() {};

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