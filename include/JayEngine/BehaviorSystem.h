#pragma once
#include "System.h"

NS_JE_BEGIN

class BehaviorSystem : public System
{
	friend class SystemManager;

public:
	
private:

	BehaviorSystem();
	~BehaviorSystem() {};
	BehaviorSystem(const BehaviorSystem& /*_copy*/) {};
	void operator=(const BehaviorSystem& /*_copy*/) {};

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

};

NS_JE_END