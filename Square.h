class Set;	// pre-declare

class Square {

	private:
		bool canHave[MAX];
		Set* hLine;
		Set* vLine;
		Set* bigSquare;
		int id;
		int assignedNumber;
		bool init;

	public:
		Square();
		//Square(Square* square);
		Square(int squareID);
		Square(int squareID, int number);
		void Init();
		void PlaceNum(int number);
		bool CanHave(int number);
		void CanHaveList(int (&list)[MAX]);
		bool CanHaveAny(vector<int>* list);
		bool Block(int number);
		int GetNumber();
		int CanHaveCount();
		int GetID();
		Set * VLine();
		Set * HLine();
		Set * BigSquare();
		
		void LinkVLine(Set* verLine);
		void LinkHLine(Set* horLine);
		void LinkBigSquare(Set* theBigSquare);
		
		static void (*onBlockCallback)(int id, int number);
};
