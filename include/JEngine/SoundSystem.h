#pragma once
#include "System.h"

jeBegin

class SoundSystem : public System
{
	friend class SystemManager;

public:

	
private:

	SoundSystem();
	virtual ~SoundSystem() {};
	SoundSystem(const SoundSystem& /*copy*/) = delete;
	void operator=(const SoundSystem& /*copy*/) = delete;

	void Load(CR_RJDoc _data) override;
	void Init() override;
	void Update(float dt) override;
	void Close() override;
	void Unload() override;

};

jeEnd
