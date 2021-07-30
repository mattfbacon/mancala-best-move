#pragma once

#include "MancalaBoard.hpp"

struct MancalaTree {
	MancalaBoard board;
	std::unique_ptr<MancalaTree> items[NUM_BINS] = {};
protected:
	MancalaTree() {}  // = default;
	MancalaTree(MancalaBoard const& board);
	MancalaTree(MancalaBoard&& board);
	std::ostream& ostream_helper(std::ostream& os, MancalaBin const bin, signed char depth = 0) const;
public:
	static MancalaTree* make(signed char num_in_each = 4);
	static MancalaTree* make(MancalaTree* const current);
	friend std::ostream& operator<<(std::ostream& os, MancalaTree const& tree);
};
