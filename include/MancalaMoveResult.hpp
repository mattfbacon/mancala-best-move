#pragma once

enum class MancalaMoveResult : signed char {
	go_again = 0,
	turn_ended,
	invalid_move = -1,
};
