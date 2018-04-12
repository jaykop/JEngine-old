#pragma once
#include "System.h"

JE_BEGIN

class SoundSystem : public System
{
	friend class SystemManager;

public:

	
private:

	SoundSystem();
	~SoundSystem() {};
	SoundSystem(const SoundSystem& /*_copy*/) = delete;
	void operator=(const SoundSystem& /*_copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(const float _dt) override;
	void Close() override;
	void Unload() override;

};

JE_END
