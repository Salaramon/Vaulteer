#pragma once

#include <corecrt_math_defines.h>
#include <type_traits>

namespace std {

	template<class Type> requires (std::is_floating_point_v<Type>)
		constexpr Type ctradians(Type deg) {
		return (M_PI / 180) * deg;
	}

	template<class Type> requires (std::is_floating_point_v<Type>)
		constexpr Type ctdegrees(Type rad) {
		return (180 / M_PI) * rad;
	}

}