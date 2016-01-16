


class Solver {

protected:

	static void (*drawNumCallback)(int id, int number);

	Square squares[MAX*MAX];
	Set sets[MAX*3];
	std::vector<std::vector<int>> numberCombos;
	bool update;
	float max;
	bool easyMode;

	// Algorithms
	bool SinglePossibilityInSquare();
	bool SinglePossibilityInSet();
	bool SubgroupExclusion();
	bool NakedSets();
	bool HiddenSets();
	bool HiddenAndNakedTwinExclusion();
	bool XWingAndSwordfish();

	// Algorithm helpers
	bool FindNakedSetRecursive(Set *set, vector<int>* used_numbers, Set* used_squares, Square* square);
	int GetSquareIdFromRegion(int id, int squarenum);

public:

	Solver();

	void SetCallbacks(void(*drawNumCallback)(int id, int number), void(*onBlockCallback)(int id, int number));
	void UpdateBoard(vector<int>* board);
	bool BoardHasDuplicates();
	void Solve(bool easyMode);
};