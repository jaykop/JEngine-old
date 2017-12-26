#pragma once
#include "UserComponent.h"
#include "ComponentBuilder.h"

#include "Vector3.h"

JE_BEGIN

class SampleLogicBuilder : public ComponentBuilder
{

	friend class AssetManager;

public:

private:

	SampleLogicBuilder();
	~SampleLogicBuilder() {};
	SampleLogicBuilder(const SampleLogicBuilder& _copy) = delete;
	void operator=(const SampleLogicBuilder& _copy) = delete;

	UserComponent* CreateComponent(Object* _pOwner) const override;

};

class SampleLogic : public UserComponent
{
	friend class ComponentManager;
	friend class SampleLogicBuilder;

public:

private:

	SampleLogic(Object* pObject);
	~SampleLogic() {};

	void Register() override;
	void Load(CR_RJValue _data) override;
	void Init() override;
	void Update(float _dt) override;
	void Close() override;
	void Unload() override;

	Object* particle;
	float	m_moveSpeed;
	bool	released;
	vec3	start, current;
};

JE_END