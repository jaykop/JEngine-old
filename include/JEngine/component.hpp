/******************************************************************************/
/*!
\file   component.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of Component class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>
#include <json_parser.hpp>

jeBegin

class Object;
class Component {

	friend class Object;

public:

	Object* get_owner() const { return owner_; }
	virtual void add_to_system() = 0;

protected:

	virtual void load(const rapidjson::Value& data) = 0;

	Component(Object* owner) : owner_(owner) {}
	virtual ~Component() {}

private:

	Object* owner_ = nullptr;
	bool bulitIn_ = false;
	std::string typeName_;

	// No default constructor
	Component() = delete;

};

jeEnd