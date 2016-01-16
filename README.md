# SudokuSolver
A tool for solving Sudoku puzzles with algorithms, no bruteforcing. 
Written in C++ with Microsoft Visual Studio 2010.


# Algorithms included

- Single possibility in square
- Single possibility in set
- Subgroup Exclusion
- Hidden & Naked sets
- Hidden & Naked Twin exclusion
- X-Wing
- Swordfish


# Just want the algorithms?
Here's what you need to use the algorithms in your own program.

**List of files**
- Solver.h
- Square.h
- Set.h
- Solver.cpp
- Square.cpp
- Set.cpp

**Important functions**

```
Solver::UpdateBoard(vector<int>* board)

  Sets internal board state for the solver. 
  Vector indexes should be in the order indicated by the picture below:
  
	+----------+----------+----------+
	| 0  1  2  | 3  4  5  | 6  7  8  |
	| 9  10 11 | 12 13 14 | 15 16 17 |
	| 18 19 20 | 21 22 23 | 24 25 26 |
	+----------+----------+----------+
	| 27 28 29 | 30 31 32 | 33 34 35 |
	| 36 37 38 | 39 40 41 | 42 43 44 |
	| 45 46 47 | 48 49 50 | 51 52 53 |
	+----------+----------+----------+
	| 54 55 56 | 57 58 59 | 60 61 62 |
	| 63 64 65 | 66 67 68 | 69 70 71 |
	| 72 73 74 | 75 76 77 | 78 79 80 |
	+----------+----------+----------+
```

```
Solver::SetCallbacks(void(*drawNumCallback)(int id, int number), void(*onBlockCallback)(int id, int number))

  Callbacks for events that happen during solving. 
  drawNumCallback is called when a number should be drawn on the interface. 
  onBlockCallback is called when a "pencilmark" number is removed from a square. 
```

```
Solver::Solve(bool easyMode)

  Solves the board.
  If easyMode is TRUE, no numbers are placed and only "pencilmark" numbers are udpated.
  You can use this to implement an "easy mode" where user can't place numbers in invalid squares.
```
