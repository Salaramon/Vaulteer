#pragma once

namespace mut {
	template<class Type> requires (std::is_integral_v<Type> || std::is_floating_point_v<Type>)
		constexpr Type ctexp2(Type value) {
		if (value != 0) {
			return 2 * ctexp2(value - 1);
		}
		else {
			return 1;
		}
	}
}