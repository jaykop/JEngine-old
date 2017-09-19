#pragma once

#include "Macro.h"

NS_JE_BEGIN

class GraphicSystem;
class PhysicsSystem;
class SoundSystem;
class BehaviorSystem;

class SystemManager {

public:

	static void Load();
	static void Init();
	static void Update(float _dt);
	static void Close();
	static void Unload();

	static void Bind();
	static void Unbind();

private:

	SystemManager();
	~SystemManager() {};
	SystemManager(const SystemManager& /*_copy*/) {};
	void operator=(const SystemManager& /*_copy*/) {};

	static GraphicSystem	*m_grpSystem;
	static PhysicsSystem	*m_phySystem;
	static SoundSystem		*m_sndSystem;
	static BehaviorSystem	*m_bhvSystem;
};

NS_JE_END