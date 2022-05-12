#pragma once

#include <vector>
#include <array>

#include "Basic.h"

namespace Vast {

	/**
	* Dynamicaly allocated array
	*/
	template<typename Ty>
	using DArray = std::vector<Ty>;

	/**
	* Static array
	*/
	template<typename Ty, uint64 Size>
	using SArray = std::array<Ty, Size>;
}