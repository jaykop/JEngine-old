#pragma once
#include <component_builder.hpp>
#include <component.hpp>
#include <vec3.hpp>

jeBegin

class Object;
jeDeclareComponentBuilder(Transform);
class Transform : public Component {

	jeBaseFriends(Transform);

public:

	vec3 position_, scale_, axis_;
	float rotation_;

protected:

	virtual void add_to_system() {};
	virtual void remove_from_system() {};
	virtual void load(const rapidjson::Value& data) {};

private:

	Transform(Object* owner);
	virtual ~Transform() {};

	Transform& operator=(const Transform& rhs);
	Transform() = delete;

};

jeEnd