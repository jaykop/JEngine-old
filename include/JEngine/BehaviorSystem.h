#pragma once
#include <vector>
#include "System.h"

jeBegin

class Object;
class CustomComponent;

class BehaviorSystem : public System
{
	friend class SystemManager;

	using Behaviors = std::vector<CustomComponent*> ;

public:

	void AddBehavior(CustomComponent* behavior);
	void RemoveBehavior(CustomComponent* behavior);

private:

	BehaviorSystem();
	virtual ~BehaviorSystem() {};
	BehaviorSystem(const BehaviorSystem& /*copy*/) = delete;
	void operator=(const BehaviorSystem& /*copy*/) = delete;

	void Load(CR_RJDoc data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

	Behaviors behaviors_;

};

jeEnd
