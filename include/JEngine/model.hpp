#pragma once
#include <component.hpp>

jeBegin

class Object;

class Graphics : protected Component {

public:

private:

	Graphics(Object* owner);
	virtual ~Graphics() {};

	Graphics() = delete;

};

jeEnd