#pragma once

#include <string>

#include "magic_enum.hpp"

constexpr signed char const NUM_BINS = 6;
enum MancalaBin : signed char { Bin_A = 0, Bin_B, Bin_C, Bin_D, Bin_E, Bin_F };
extern std::string BIN_LETTERS[NUM_BINS];

union BinIterator {
	magic_enum::underlying_type_t<MancalaBin> as_int = 0;
	MancalaBin as_bin;
};
