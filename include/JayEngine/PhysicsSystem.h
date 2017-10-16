#pragma once
#include "System.h"

NS_JE_BEGIN

class PhysicsSystem : public System
{
	friend class SystemManager;

public:
	
private:

	PhysicsSystem();
	~PhysicsSystem() {};
	PhysicsSystem(const PhysicsSystem& /*_copy*/) {};
	void operator=(const PhysicsSystem& /*_copy*/) {};

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

};

NS_JE_END