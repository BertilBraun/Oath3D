#pragma once

#include "Datatype/Vector.h"
#include <iterator>
#include <numeric>

namespace Oath {

	template<typename T = int>
	static Vector<T> range(T start, T end) {
		std::vector<T> r((size_t)(end - start));
		std::iota(r.begin(), r.end(), T(start));
		return r;
	}

	template<typename T = int>
	static Vector<T> range(T end) {
		std::vector<T> r((size_t)end);
		std::iota(r.begin(), r.end(), T(0));
		return r;
	}

}