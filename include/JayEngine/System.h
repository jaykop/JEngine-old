#pragma once

#include "Macro.h"

NS_JE_BEGIN

class System {

public:

	virtual void Load() = 0;
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

protected:

	System() {};
	~System() {};

private:


};

NS_JE_END