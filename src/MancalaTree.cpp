#include <iostream>
#include <memory>

#include "magic_enum.hpp"

#include "MancalaTree.hpp"

MancalaTree::MancalaTree(MancalaBoard const& board) : board(board) {}
MancalaTree::MancalaTree(MancalaBoard&& board) : board(std::move(board)) {}

MancalaTree* MancalaTree::make(signed char num_in_each /* = 4 */) {
	return MancalaTree::make(new MancalaTree{ MancalaBoard{ num_in_each } });
}

MancalaTree* MancalaTree::make(MancalaTree* const current /* = new MancalaTree */) {
#pragma omp parallel
#pragma omp for
	for (size_t i = 0; i < NUM_BINS; i++) {
		if (current->board.our_side[i] == 0) {  // if the bin is empty
			// we can't pick it up, so skip this one (leaving the child as a nullptr).
			current->items[i] = nullptr;
			continue;
		} else {
			current->items[i] = std::unique_ptr<MancalaTree>{ new MancalaTree{ MancalaBoard{ current->board } } };
			auto& child = current->items[i];
			auto const move_result = child->board.move(static_cast<MancalaBin>(i));
			switch (move_result) {
				case MancalaMoveResult::turn_ended: {
					// do not recurse (so all of the children are null)
					continue;
				}
				case MancalaMoveResult::go_again: {
					// do recurse
					MancalaTree::make(child.get());
					continue;
				}
				case MancalaMoveResult::invalid_move: {
					assert(false);
					continue;
				}
			}
		}
	}
	return current;
};

std::ostream& operator<<(std::ostream& os, MancalaTree const& tree) {
	for (BinIterator i = { .as_int = 0 }; i.as_int < NUM_BINS; i.as_int++) {
		if (auto* const item = tree.items[i.as_int].get(); item != nullptr) {
			item->ostream_helper(os, i.as_bin);
		}
	}
	return os;
}

std::ostream& MancalaTree::ostream_helper(std::ostream& os, MancalaBin const bin, signed char const depth /* = 0 */) const {
	// +x prints char x as number
	for (signed char i = 0; i < depth; i++) {
		os << "  ";
	}
	os << BIN_LETTERS[bin] << ' ' << +board.our_bin << std::endl;
	for (BinIterator i = { .as_int = 0 }; i.as_int < NUM_BINS; i.as_int++) {
		if (auto* const item = items[i.as_int].get(); item != nullptr) {
			item->ostream_helper(os, i.as_bin, depth + 1);
		}
	}
	return os;
}
