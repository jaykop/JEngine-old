#pragma once
#include "Macro.h"

JE_BEGIN

class System {

public:

protected:

	virtual void Load() = 0;
	virtual void Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Close() = 0;
	virtual void Unload() = 0;

	System() {};
	~System() {};

private:

	System(const System& /*_copy*/) {};
	void operator=(const System& /*_copy*/) {};
};

JE_END
