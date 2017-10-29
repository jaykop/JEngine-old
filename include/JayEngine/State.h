#pragma once
#include <string>
#include "Macro.h"

JE_BEGIN

class State {

	friend class StateManager;
	friend class AssetManager;
	friend class ObjectContainer;

public:

private:

	State(const char* _name);
	~State() {};
	State(const State& /*_copy*/) {};
	void operator=(const State& /*_copy*/) {};

	void Load();
	void Init();
	void Update(float _dt);
	void Close();
	void Unload();

	void ClearObjectContainer();

	State*				m_pLastStage;
	bool				m_paused;
	std::string			m_name, m_loadDirectory;
	ObjectContainer*	m_objContainer;
};

JE_END
