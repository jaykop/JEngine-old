#pragma once
#include <string>
#include "Macro.h"

NS_JE_BEGIN

class State {

public:

	State(const char* _name);
	~State() {};

	void Load();
	void Init();
	void Update(float _dt);
	void Close();
	void Unload();

private:

	friend class StateManager;

	State(const State& /*_copy*/) {};
	void operator=(const State& /*_copy*/) {};

	State*		m_pLastStage;
	bool		m_paused;
	std::string m_name;
};

NS_JE_END