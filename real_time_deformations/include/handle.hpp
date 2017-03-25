#pragma once

#include <atlas/math/Math.hpp>

class handle {
public:
	handle() = delete;
	
	virtual atlas::math::Matrix4 generate_transformations() = 0;
};