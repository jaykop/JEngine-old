#pragma once
#include "System.h"
#include <unordered_map>
JE_BEGIN

class Object;
typedef std::unordered_map<std::string, Object*> ObjectMap;

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

	ObjectMap* m_pObjectMap;

};

JE_END
