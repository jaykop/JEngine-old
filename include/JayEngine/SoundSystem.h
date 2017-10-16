#pragma once
#include "System.h"

NS_JE_BEGIN

class SoundSystem : public System
{
	friend class SystemManager;

public:

	
private:

	SoundSystem();
	~SoundSystem() {};
	SoundSystem(const SoundSystem& /*_copy*/) {};
	void operator=(const SoundSystem& /*_copy*/) {};

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

};

NS_JE_END