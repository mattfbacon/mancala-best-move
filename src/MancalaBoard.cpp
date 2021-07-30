#include <algorithm>
#include <cstring>
#include <iostream>

#include "MancalaBoard.hpp"

MancalaBoard::MancalaBoard(signed char const num_in_each /* = 4 */) {
	memset(our_side, num_in_each, sizeof our_side);
	memset(their_side, num_in_each, sizeof their_side);
}

MancalaBoard::MancalaBoard(MancalaBoard const& other) {
	// this exists since mutexes can't be copied.
	// (board_mutex is default-constructed)
	our_bin = other.our_bin;
	their_bin = other.their_bin;
	memcpy(our_side, other.our_side, sizeof other.our_side);
	memcpy(their_side, other.their_side, sizeof other.their_side);
}

void swap(MancalaBoard& a, MancalaBoard& b) noexcept {
	std::scoped_lock board_locks(a.board_mutex, b.board_mutex);
	using std::swap;
	swap(a.our_bin, b.our_bin);
	swap(a.their_bin, b.their_bin);
	swap(a.our_side, b.our_side);
	swap(a.their_side, b.their_side);
};

MancalaBoard& MancalaBoard::operator=(MancalaBoard src) {
	std::swap(*this, src);
	return *this;
}

[[nodiscard]] MancalaMoveResult MancalaBoard::move(MancalaBin const bin) {
	size_t i = bin;
	// just in case
	std::lock_guard<std::mutex> board_lock(board_mutex);
	bool on_our_side = true;
	signed char hand_count = our_side[i];
	our_side[i] = 0;
	if (hand_count == 0) {
		return MancalaMoveResult::invalid_move;
	}
	// PSEUDOCODE:
	// after picking up the bin (above code), move forward one.
	// if the current bin is our bin, put one in it, and if that one was our last, we get another turn.
	// if not wrapping around, i.e., on a regular bin,, drop one in the bin.
	// then if that was our last one, and if the bin was empty before, our turn is over; otherwise (if it was our last but the bin wasn't empty) pick up that bin and keep going.
	for (;;) {
		i++;
		if (i >= NUM_BINS) {  // if we are wrapping around
			if (on_our_side) {  // if we are on our side
				// drop one in our bin
				our_bin++;
				hand_count--;
				if (hand_count == 0) {  // if the last one was in our bin
					return MancalaMoveResult::go_again;
				}
			}
			// go to the other side
			on_our_side = !on_our_side;
			i = -1;  // since we start with i++, this will make i 0 to start
			// and keep going
			continue;
		} else {  // not wrapping around
			// drop one in the bin
			auto* const current_bin = &(on_our_side ? our_side : their_side)[i];
			(*current_bin)++;
			hand_count--;
			if (hand_count == 0) {  // if we ran out on this one
				if (*current_bin == 1) {  // if it was empty before
					return MancalaMoveResult::turn_ended;
				} else {  // it was not empty before
					// pick it up
					hand_count = *current_bin;
					*current_bin = 0;
					// and keep going
					continue;
				}
			}
		}
	}
}

signed char MancalaBoard::bin_value(MancalaBin const bin) const {
	std::lock_guard<std::mutex> board_lock(board_mutex);
	return our_side[*magic_enum::enum_index(bin)];
}

signed char MancalaBoard::get_ours() const {
	return our_bin;
}

std::ostream& operator<<(std::ostream& os, MancalaBoard const& board) {
	// +x prints char x as number
	auto const old_width = os.width();
	auto const old_flags = os.flags();
	os << '|';
	os.setf(std::ios::right | std::ios::dec);
	for (size_t i = 0; i < NUM_BINS; i++) {
		os << ' ';
		os.width(2);
		os << +board.their_side[NUM_BINS - i - 1];
	}
	os << " |" << std::endl;
	os << '|';
	for (size_t i = 0; i < NUM_BINS; i++) {
		os << ' ';
		os.width(2);
		os << +board.our_side[i];
	}
	os << " |" << std::endl;
	os << "Theirs " << +board.their_bin << " | Ours " << +board.our_bin;
	os.width(old_width);
	os.flags(old_flags);
	return os;
}
