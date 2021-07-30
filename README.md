# Mancala Simulator (Limited)

The purpose of this project is to find the best starting move for Mancala. It
tells you the move you can do to get the highest score for games starting with
3 or 4 stones in each bin.

## Build and Run

```bash
make debug
./dist/Debug/mancala
# or
make release
./dist/Release/mancala
```

## Technologies/Libraries Used

- CMake
- Ninja
- OpenMP
- LLD
- Clang
- Magic Enum
- Git Submodules

## Data Structures Used

- Linked List
- Senary (6-ary) Tree

## Algorithms Used

- Mancala Moves (obviously)
- Recursive Tree Generation/Walking
	- Tree Max with Path
	- Generate Tree Based on Result of Function
