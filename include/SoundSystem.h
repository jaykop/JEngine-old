#pragma once
#include "System.h"

NS_JE_BEGIN

class SoundSystem : public System
{

public:

	friend class SystemManager;

	void Load() override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

private:

	SoundSystem();
	~SoundSystem() {};
	SoundSystem(const SoundSystem& /*_copy*/) {};
	void operator=(const SoundSystem& /*_copy*/) {};
\
};

NS_JE_END