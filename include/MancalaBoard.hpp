#pragma once

namespace std {
typedef basic_ostream<char> ostream;
};

#include <mutex>

#include "magic_enum.hpp"

struct MancalaTree;

#include "MancalaBin.hpp"
#include "MancalaMoveResult.hpp"

class MancalaBoard {
public:
	explicit MancalaBoard(signed char num_in_each = 3);
	MancalaBoard(MancalaBoard const& other);
	MancalaBoard& operator=(MancalaBoard other);

	[[nodiscard]] MancalaMoveResult move(MancalaBin bin);
	signed char bin_value(MancalaBin bin) const;
	signed char get_ours() const;
protected:
	std::mutex mutable board_mutex;

	signed char our_bin = 0;
	signed char their_bin = 0;
	signed char our_side[NUM_BINS];
	// their_side is indexed from right-to-left:
	// | 5 4 3 2 1 0 |
	signed char their_side[NUM_BINS];

	friend void swap(MancalaBoard& a, MancalaBoard& b) noexcept;
	friend std::ostream& operator<<(std::ostream& os, MancalaBoard const& board);
	friend struct MancalaTree;
};
