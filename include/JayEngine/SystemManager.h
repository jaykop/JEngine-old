#pragma once
#include <stack>
#include "Macro.h"

NS_JE_BEGIN

class SoundSystem;
class GraphicSystem;
class PhysicsSystem;
class BehaviorSystem;

class SystemManager {

	typedef std::stack<SystemManager*> SystemStack;

	friend class State;
	friend class StateManager;

public:

	static SoundSystem		*m_pSoundSystem;
	static GraphicSystem	*m_pGraphicSystem;
	static PhysicsSystem	*m_pPhysicsSystem;
	static BehaviorSystem	*m_pBehaviorSystem;

private:

	static void Load();
	static void Init();
	static void Update(float _dt);
	static void Close();
	static void Unload();

	static void Pause();
	static void Resume();

	static void Bind();
	static void Unbind();

	SystemManager();
	~SystemManager() {};
	SystemManager(const SystemManager& /*_copy*/) {};
	void operator=(const SystemManager& /*_copy*/) {};

	static bool			m_binded;
	static SystemStack	m_pauseStack;
};

NS_JE_END