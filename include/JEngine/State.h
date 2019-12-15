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

	State(const char* name);
	~State() {};
	State() = delete;
	State(const State& /*copy*/) = delete;
	void operator=(const State& /*copy*/) = delete;

	void Load();
	void Init();
	void Update(float dt);
	void Close();
	void Unload();

	void ClearObjectContainer();

	State*				pLastStage_;
	std::string			name_, loadDirectory_;
	ObjectContainer*	pObjContainer_;
};

jeEnd
