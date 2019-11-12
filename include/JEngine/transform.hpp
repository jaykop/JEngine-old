#pragma once
#include <component.hpp>
#include <vec3.hpp>

jeBegin

class Object;

class Transform : protected Component {

public:

	vec3 position_, scale_, axis_;
	float rotation_;

private:

	Transform(Object* owner);
	virtual ~Transform() {};

	Transform& operator=(const Transform& rhs);
	Transform() = delete;

};

jeEnd