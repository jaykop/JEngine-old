#pragma once
#include <string>
#include "Macro.h"

jeBegin

class ObjectContainer;

class State {

	friend class StateManager;
	friend class AssetManager;

public:

	const char * GetName() const;

private:

	State(const char* _name);
	~State() {};
	State() = delete;
	State(const State& /*_copy*/) = delete;
	void operator=(const State& /*_copy*/) = delete;

	void Load();
	void Init();
	void Update(float dt);
	void Close();
	void Unload();

	void ClearObjectContainer();

	State*				m_pLastStage;
	bool				m_paused;
	std::string			m_name, m_loadDirectory;
	ObjectContainer*	m_pObjContainer;
};

jeEnd
