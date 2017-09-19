#pragma once
#include "System.h"

NS_JE_BEGIN

class PhysicsSystem : public System
{

public:

	friend class SystemManager;

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

private:

	PhysicsSystem();
	~PhysicsSystem() {};
	PhysicsSystem(const PhysicsSystem& /*_copy*/) {};
	void operator=(const PhysicsSystem& /*_copy*/) {};


};

NS_JE_END