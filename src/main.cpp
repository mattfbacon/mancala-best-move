#include <iostream>
#include <list>
#include <memory>
#include <utility>

#include "MancalaTree.hpp"

// returns the moves in REVERSE ORDER
std::pair<std::list<MancalaBin>, signed char> tree_max(MancalaTree* root, std::list<MancalaBin> const& current_path = {}) {
	// Base case
	if (root == nullptr) {
		return { current_path, std::numeric_limits<signed char>::min() };
	}
	std::mutex max_mutex;
	auto current_max = std::pair{ current_path, root->board.get_ours() };
#pragma omp parallel
#pragma omp for
	for (size_t i = 0; i < NUM_BINS; i++) {
		if (auto const& item = root->items[i]; item.get() != nullptr) {
			std::lock_guard<std::mutex> max_guard{ max_mutex };
			auto their_path = current_path;  // copy
			their_path.emplace_front(static_cast<MancalaBin>(i));
			if (auto const new_max = tree_max(item.get(), their_path); new_max.second > current_max.second) {
				current_max = new_max;
			}
		}
	}
	return current_max;
}

void print_max(signed char num_in_each) {
	std::unique_ptr<MancalaTree> const tree{ MancalaTree::make(num_in_each) };
	auto const max = tree_max(tree.get());
	auto const end_iter = std::crend(max.first);
	MancalaBoard board{ num_in_each };
	bool invalid = false;
	for (auto iter = std::crbegin(max.first); iter != end_iter; iter++) {
		std::cout << BIN_LETTERS[*iter];
		if (!invalid && board.move(*iter) == MancalaMoveResult::turn_ended && board.get_ours() != max.second) {
			std::clog << +board.get_ours() << std::endl;
			invalid = true;
		}
	}
	std::cout << " with a total of " << +max.second << " stones." << std::endl;
	std::cout << "This solution was found to be " << (invalid ? "INVALID" : "valid") << '.' << std::endl;
}

int main() {
	for (signed char num_in_each : { 3, 4 }) {
		std::cout << "Max for " << +num_in_each << ':' << std::endl;
		print_max(num_in_each);
	}
	return 0;
}
