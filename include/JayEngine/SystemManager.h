#pragma once
#include <stack>
#include "Macro.h"

NS_JE_BEGIN

class SoundSystem;
class GraphicSystem;
class PhysicsSystem;
class BehaviorSystem;


class SystemManager {

	class Systems {

		friend class SystemManager;

	private:

		Systems();
		~Systems() {};
		Systems(const Systems& /*_cpoy*/) {};
		void operator=(const Systems& /*_cpoy*/) {};

		void Bind();
		void Unbind();

		void Load();
		void Init();
		void Update(float _dt);
		void Close();
		void Unload();

		SoundSystem		*m_pSoundSystem;
		GraphicSystem	*m_pGraphicSystem;
		PhysicsSystem	*m_pPhysicsSystem;
		BehaviorSystem	*m_pBehaviorSystem;

	};

	friend class State;
	friend class StateManager;

	typedef std::stack<Systems*> SystemStack;

public:

	static Systems				*m_systems;

	static SoundSystem*			GetSoundSystem();
	static GraphicSystem*		GetGraphicSystem();
	static PhysicsSystem*		GetPhysicsSystem();
	static BehaviorSystem*		GetBehaviorSystem();

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

	static SystemStack	m_pauseStack;
};

NS_JE_END