class Square;	// pre-declare

class Set {

	private:
		Square* squares[MAX];
		int filled;

	public:
		Set();
		void Add(Square* square);
		void RemoveLast();
		Set GetPossibilitiesFor(int number);
		Square* GetSquare(int num);
		int Filled();
		bool LocationsMatch(Set* toBeCompared);
		bool HiddenMultiples(Set** toBeComparedList, int sets, Set* squaresMet);
		bool LocationsOverlap(Set* toBeCompared);
		bool LocationsLine(Set* toBeCompared, bool HLine);
		bool BlockAllExcluding(bool* matches);
		bool BlockAllExcluding(Set* set, int number);
		bool BlockAllOthers(bool* matches, Set* set);
		bool HasDuplicates();
		bool HasSquare(Square* square);
		bool LinksWith(Set* set, bool vertically, Set* used);
		bool Combine(Set* set);
};
