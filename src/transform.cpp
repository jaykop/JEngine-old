#include <transform.hpp>

jeBegin

Transform::Transform(Object* owner) 
: Component(owner), position_(vec3::zero), scale_(vec3::zero),
rotation_(0.f), axis_(vec3(0, 0, 1)) {};

Transform& Transform::operator= (const Transform& rhs) {

	position_.set(rhs.position_);
	scale_.set(rhs.scale_);
	rotation_ = rhs.rotation_;
	axis_.set(rhs.axis_);

	return *this;
}

jeEnd