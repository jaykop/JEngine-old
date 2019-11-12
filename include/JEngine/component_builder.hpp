/******************************************************************************/
/*!
\file   component_builder.hpp
\author Jeong Juyong
\par    email: jaykop.jy\@gmail.com
\date   2019/06/08(yy/mm/dd)

\description
Contains the definition of ComponentBuilder class
*/
/******************************************************************************/

#pragma once
#include <macros.hpp>

jeBegin

class Object;
class Component;

class ComponentBuilder {

	friend class ComponentManager;

public:

	virtual Component* create_component() const = 0;

protected:

	ComponentBuilder() {};
	virtual ~ComponentBuilder() {};

private:

	// Prevent to clone this class
	ComponentBuilder(ComponentBuilder&&) = delete;
	ComponentBuilder(const ComponentBuilder&) = delete;
	ComponentBuilder& operator=(ComponentBuilder&&) = delete;
	ComponentBuilder& operator=(const ComponentBuilder&) = delete;

};

jeEnd