
#include "StdAfx.h"
#include <algorithm>

void (*Solver::drawNumCallback)(int id, int number) = NULL;

Solver::Solver() {

	max = sqrt( static_cast<float>(MAX) );	// sqrt(9) = 3

	// Build board and sets
	for (unsigned int i = 0; i < MAX*MAX; i++) {
		squares[i] = Square( i );
	}

	for (int i = 0; i < MAX; i++) {

		for (int y = 0; y < MAX; y++) {

			// horizontal
			sets[i].Add		( &squares[ y + (i * MAX) ] );		
			squares[ y + (i * MAX) ].LinkHLine( &sets[i] );

			// vertical
			sets[i+MAX].Add	( &squares[ (y * MAX) + i ] );		
			squares[ (y * MAX) + i ].LinkVLine( &sets[i+MAX] );

			// bigsquare
			sets[i+MAX*2].Add	( &squares[ GetSquareIdFromRegion(i,y) ] );	
			squares[ GetSquareIdFromRegion(i,y) ].LinkBigSquare( &sets[i+MAX*2] );
	
		}
	}

	// Build a list of possible number combinations
	for (int combosize = 2; combosize <= 5; combosize++) {
				
		std::string bitmask(combosize, 1); // K leading 1's
		bitmask.resize(MAX, 0); // N-K trailing 0's

		// save integers and permute bitmask
		do {

			std::vector<int> combo;

			for (int i = 0; i < MAX; ++i) // [0..N-1] integers
			{
				if (bitmask[i]) {
					combo.push_back(i);
				}
			}
			numberCombos.push_back(combo);
		} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
	}
}

void Solver::SetCallbacks(void(*drawNumCallback)(int id, int number), void(*onBlockCallback)(int id, int number)) {

	this->drawNumCallback = drawNumCallback;

	Square::onBlockCallback = onBlockCallback;
}

void Solver::UpdateBoard(vector<int>* board) {

	if (board->size() != MAX*MAX)
		return;

	for (unsigned int i = 0; i < MAX*MAX; i++) {
		squares[i].Reset();
	}

	for (unsigned int i = 0; i < MAX*MAX; i++) {
		squares[i].PlaceNum((*board)[i]);
	}
}

bool Solver::BoardHasDuplicates() {

	// Check for duplicate numbers in the sets
	for (int i = 0; i < MAX*3; i++) {
		if (sets[i].HasDuplicates()) {
			return true;
		}
	}
	return false;
}

void Solver::Solve(bool easyMode) {

	this->easyMode = easyMode;

	// Run algorithms until none can modify the current board state
	do {
		update = false;

		// Continue if a simple algorithm made a change to optimize solving speed
		if (SinglePossibilityInSquare()) continue;
		if (SinglePossibilityInSet()) continue;
		if (SubgroupExclusion()) continue;
		NakedSets();
		HiddenSets();
		HiddenAndNakedTwinExclusion();
		XWingAndSwordfish();
		
	} while (update);
}

/*	
    Indexes for each small square
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


	Indexes for bigger square sets
	+----------+----------+----------+
	|          |          |          |
	|    0     |    1     |    2     |
	|          |          |          |
	+----------+----------+----------+
	|          |          |          |
	|    3     |    4     |    5     |
	|          |          |          |
	+----------+----------+----------+
	|          |          |          |
	|    6     |    7     |    8     |
	|          |          |          |
	+----------+----------+----------+
*/

/////////////
// ALGORITHMS
///////////////

/////////////////////////////
// "Single possibility in square"
// If only one number fits in the square, put it there
////////////////////////////
bool Solver::SinglePossibilityInSquare() {

	if (easyMode)
		return false;

	for (int i = 0; i < MAX*MAX; i++) {
		if (squares[i].GetNumber() == 0) {
			int onlyNum;
			int possibilities = 0;
			for (int number = 1; number <= MAX; number++) {
				if (squares[i].CanHave(number)) {
					onlyNum = number;
					possibilities++;
				}
			}
			if (possibilities == 1) {
				squares[i].PlaceNum(onlyNum);
				update = true;

				if (drawNumCallback != NULL)
					(*drawNumCallback)(squares[i].GetID(), onlyNum);
			}
		}
	}

	return update;
}

/////////////////////////////
// "Single possibility in set"
// If a number can only be in one square of a set, put it there
////////////////////////////
bool Solver::SinglePossibilityInSet() {

	if (easyMode)
		return false;

	for (int number = 1; number <= MAX; number++) {
		for (int set = 0; set < MAX*3; set++) {

			Set possibilities = sets[set].GetPossibilitiesFor(number);

			if (!easyMode) {
				if (possibilities.Filled() == 1) {
					possibilities.GetSquare(0)->PlaceNum(number);
					update = true;

					if (drawNumCallback != NULL)
						(*drawNumCallback)(possibilities.GetSquare(0)->GetID(), number);
				}
			}
		}
	}
	return update;
}

//////////////////////////////
// "Subgroup exclusion"
// If all possibilities for a number in a big square are in the same horizontal or vertical line,
// remove the possibility from the horizontal and vertical lines outside the big square
//    AND
// If all the possibilities for a number on a vertical or horizontal line are in the same big square,
// remove the possibility from other squares in that big square
//////////////////////////////
bool Solver::SubgroupExclusion() {

	for (int number = 1; number <= MAX; number++) {
		for (int set = 0; set < MAX*3; set++) {
			
			Set possibilities = sets[set].GetPossibilitiesFor(number);

			if (possibilities.Filled() > 1 && possibilities.Filled() <= max) {
					
				// Processing big square
				if (set < MAX*2) {

					// Confirm all possibilities are in a big sqare
					if (possibilities.LocationsOverlap(possibilities.GetSquare(0)->BigSquare() ) )

						update = possibilities.GetSquare(0)->BigSquare()->BlockAllExcluding(&possibilities,number) || update;
				}

				// Processing horizontal or vertical line
				else if (set >= MAX*2) {

					if (possibilities.LocationsOverlap(possibilities.GetSquare(0)->HLine() ) )
						update = possibilities.GetSquare(0)->HLine()->BlockAllExcluding(possibilities.GetSquare(0)->BigSquare(), number ) || update;

					else if (possibilities.LocationsOverlap(possibilities.GetSquare(0)->VLine() ) )
						update = possibilities.GetSquare(0)->VLine()->BlockAllExcluding(possibilities.GetSquare(0)->BigSquare(), number ) || update;
				}
			}
		}
	}
	return update;
}

// Naked pairs, triples, quads...
bool Solver::NakedSets() {

	for (int set = 0; set < MAX*3; set++) {		

		for (int j = 0; j < MAX; j++) {

			Square * square = sets[set].GetSquare(j);
			int canHave = square->CanHaveCount();

			if (canHave < 2 || canHave >= MAX - sets[set].Filled())
				continue;

			Set used_squares;
			vector<int> used_numbers;
						
			if (FindNakedSetRecursive( &sets[set], &used_numbers, &used_squares, square ) ) {
				for (unsigned int k = 0; k < used_numbers.size(); k++) {
					update = sets[set].BlockAllExcluding(&used_squares,used_numbers.at(k)) || update;
				}
			}
		}
	}
	
//	if (update)
//		MessageBox("naked2",NULL,MB_OK);

	return update;
}

// Hidden pairs, triples, quads... v2
bool Solver::HiddenSets() {

	for (int set = 0; set < MAX*3; set++) {
			
		Set numberSets[MAX];

		for (int number = 1; number <= MAX; number++) {
			numberSets[number-1] = sets[set].GetPossibilitiesFor(number);
		}

		for (unsigned int i = 0; i < numberCombos.size(); i++) {
				
			int multipleCount = numberCombos[i].size();
			Set** hiddenMultiples = new Set*[multipleCount-1];
					
			Set squaresMet;
			bool numberMatches[MAX];

			for (int k = 0; k < MAX; k++)
				numberMatches[k] = false;

			numberMatches[numberCombos[i][0]] = true;

			for (int j = 1; j < multipleCount; j++) {
				hiddenMultiples[j-1] = &numberSets[numberCombos[i][j]];
				numberMatches[numberCombos[i][j]] = true;
			}
					
			if ( numberSets[numberCombos[i][0]].HiddenMultiples( hiddenMultiples, multipleCount-1, &squaresMet ) ) {					
				update = squaresMet.BlockAllExcluding(numberMatches) || update;
			}
			delete [] hiddenMultiples;
		}
	}
	//if (update)
	//	MessageBox("hidden multiples",NULL,MB_OK);

	return update;
}

/////////////////////////////////////
// "Hidden & Naked Twin exclusion" //
/////////////////////////////////////
bool Solver::HiddenAndNakedTwinExclusion() {

	for (int set = 0; set < MAX*3; set++) {

		// Get possibilities for each number in the set
		Set possibilities[MAX];
		for (int number = 1; number <= MAX; number++)
			possibilities[number-1] = sets[set].GetPossibilitiesFor(number);
				
				
		for (int number1 = 0; number1 < MAX; number1++) {

			// Get matching and overlapping number candidates
			int matched = 1;
			bool matches[MAX];
			matches[number1] = true;

			int overlapped = 1;
			bool overlaps[MAX];
			overlaps[number1] = true;

			for (int number2 = 0; number2 < MAX; number2++) {
				if (number1 != number2) {
					if (possibilities[number1].LocationsMatch( &possibilities[number2] ) ) {
						matches[number2] = true;
						matched++;
					}
					else
						matches[number2] = false;

					if (possibilities[number1].LocationsOverlap( &possibilities[number2] ) ) {
						overlaps[number2] = true;
						overlapped++;
					}
					else
						overlaps[number2] = false;

				}
			}

			// If matching candidates were found, block all but the candidates themselves
			if (matched > 1 && matched == possibilities[number1].Filled()) {
				update = possibilities[number1].BlockAllExcluding(matches) || update;
			}

			// See if overlapping candidates are the only possibilities in their respcctive squares
			// If so, block the candidates from other related sets
			if (overlapped > 1 && overlapped == possibilities[number1].Filled()) {

				bool noOthers = true;

				for (int i = 0; i < overlapped; i++) {
					if (possibilities[number1].GetSquare(i)->CanHaveCount() != overlapped) {
						noOthers =  false;
						break;
					}
				}
				if (noOthers)
					update = possibilities[number1].BlockAllOthers(overlaps,&sets[set]) || update;
			}
		}
	}
	/*if (update) {
		MessageBox("twin exclusion",NULL,MB_OK);
	}*/

	return update;
}

			
bool Solver::XWingAndSwordfish() {

	for (int number = 1; number <= MAX; number++) {

		// Only horizontal and vertical lines need to be checked (not the big squares)
		// Use 'horizontally' as a boolean to differentiate horizontal vs vertical lining
		for (unsigned int horizontally = 0; horizontally <= 1; horizontally++) {

			// Allocate space for bonded sets with 2-8 possible squares, not 1 or 9, hence -2
			int NumberOfBondedSets[MAX-2];
			Set BondedSets[MAX-2][MAX];
			for (int y = 0; y < MAX-2; y++)
				NumberOfBondedSets[y] = 0;


			// Create helper tables we can use to search if squares are on same horizontal or vertical line
			for (int set = 0; set < MAX; set++) {
				Set tmpSet = sets[horizontally*MAX + set].GetPossibilitiesFor(number);
				int section = tmpSet.Filled()-2;
				if (section >= 0 && section < MAX-2) {
					BondedSets[ section ][ NumberOfBondedSets [ section ] ] = tmpSet;
					NumberOfBondedSets [ section ]++;	
				}
			}

			//////////////////////
			// X-Wing
			///////////////////////
			for (int y = 0; y < MAX-2; y++) {
				if (NumberOfBondedSets[y] == y+2) {	// as many possibilities on line as there are total lines

					bool linesUp = true;

					// Figure out which type of line (horizontal vs vertical) do the possibilities form, if any
					for (int set = 1; set < NumberOfBondedSets[y]; set++) {
						if (!BondedSets[y][0].LocationsLine(&BondedSets[y][set], horizontally)) {
							linesUp = false;
							break;
						}
					} 

					if (linesUp) {

						for (int square = 0; square < NumberOfBondedSets[y]; square++) {

							Set lineToBlock = BondedSets[y][0].GetSquare(square)->Line(horizontally)->GetPossibilitiesFor(number);

							// Check if there is anything to block on the line
							if (lineToBlock.Filled() > NumberOfBondedSets[y]) {

								for (int bSquare = 0; bSquare < lineToBlock.Filled(); bSquare++) {

									// ... But don't block squares on the line that formed the X-Wing
									bool needsBlocking = true;
										
									for (int set = 0; set < NumberOfBondedSets[y]; set++) {

										if (lineToBlock.GetSquare(bSquare) == BondedSets[y][set].GetSquare(square)) {
											needsBlocking = false;
											break;
										}
									}

									if (needsBlocking) {
										update = lineToBlock.GetSquare(bSquare)->Block(number) || update;
									}
								}
							}	
						}
					}
							
				}
			}

			/*if (update) {
				MessageBox("xwing",NULL,MB_OK);
			}*/

			/////////////
			// Swordfish (uses BondedSets initialized by X-Wing)
			/////////////
			if (!update && NumberOfBondedSets[0] + NumberOfBondedSets[1] >= 3) {
						
				bool vertically = (horizontally == 0);
						
				int sf_setsNum = NumberOfBondedSets[0] + NumberOfBondedSets[1];
				Set** sf_sets = new Set*[sf_setsNum];

				int k = 0;

				for (int j = 0; j < NumberOfBondedSets[0]; j++) {
					sf_sets[k] = &BondedSets[0][j];
					k++;
				}
				for (int j = 0; j < NumberOfBondedSets[1]; j++) {
					sf_sets[k] = &BondedSets[1][j];
					k++;
				}

				int set[3];

				for (set[0] = 0; set[0] < sf_setsNum; set[0]++) {
					for (set[1] = 0; set[1] < sf_setsNum; set[1]++) {
						if (set[0] != set[1]) {

							Set used = Set();

							if ( sf_sets[set[0]]->LinksWith( sf_sets[set[1]], vertically, &used ) ) {
	
								for (set[2] = 0; set[2] < sf_setsNum; set[2]++) {

									if (set[2] != set[0] && 
										set[2] != set[1]) {

										Set used_tmp = used;

										if (sf_sets[set[2]]->LinksWith( sf_sets[set[0]], vertically, &used_tmp ) &&
											sf_sets[set[2]]->LinksWith( sf_sets[set[1]], vertically, &used_tmp ) ) {
												
											// Check all of set's squares are included in the link
											if (used_tmp.Filled() ==	sf_sets[set[0]]->Filled() + 
																		sf_sets[set[1]]->Filled() + 
																		sf_sets[set[2]]->Filled()) {
														
												// swordfish found, start restricting...

												for (int j = 0; j < 3; j++) {
													for (int k = 0; k < sf_sets[set[j]]->Filled(); k++) {
														update = sf_sets[set[j]]->GetSquare(k)->Line(horizontally)->BlockAllExcluding(&used_tmp,number) || update;
													}

												}
											}

										}					
									}
								}
							}
						}
					}
				}

				delete [] sf_sets;

				/*if (update)
					MessageBox("swordfish",NULL,MB_OK);
				*/
			}
			/////////////////
		}
	}
	return update;
}

/////////////////
// ALGORITHM HELPERS
/////////////////
bool Solver::FindNakedSetRecursive(Set *set, vector<int>* used_numbers, Set* used_squares, Square* square) {

	// Add the square to the link
	used_squares->Add(square);

	// Add square' numbers
	for (int i = 1; i <= MAX; i++) {
		if (square->CanHave(i))
			used_numbers->push_back(i);
	}

	int differentNumbers = 0;
	bool numbers[MAX];

	for (int i = 0; i < MAX; i++)
		numbers[i] = false;

	// Count the amount of unique numbers
	for (unsigned int i = 0; i < used_numbers->size(); i++) {
		int index = used_numbers->at(i)-1;
		if (!numbers[index]) {
			numbers[index] = true;
			differentNumbers++;
		}
	}

	// If there are as many squares as there are unique numbers, we have found a naked set
	if (used_squares->Filled() == differentNumbers)
		return true;

	// Add a square to the link containing a number from the set
	else {
		bool ok = false;
		for (int i = 0; i < set->Filled(); i++) {
			Square* set_square = set->GetSquare(i);
			if (!used_squares->HasSquare(set_square)) {
				if (set_square->CanHaveAny(used_numbers)) {
					if (FindNakedSetRecursive( set, used_numbers, used_squares, set_square)) {
						ok = true;
						break;
					}
				}
			}
		}
		if (!ok) {
			
			int numbers = used_squares->GetSquare(used_squares->Filled()-1)->CanHaveCount();

			for (int i = 0; i < numbers; i++ ) {
				used_numbers->pop_back();
			}

			used_squares->RemoveLast();
			return false;
		}
	}
	return true;
}

int Solver::GetSquareIdFromRegion(int id, int squarenum) {

	int squareroot = static_cast<int>(sqrt( static_cast<float>(MAX) ));	// sqrt(9) = 3

	div_t divId = div(id,squareroot);
	div_t divSquareNum = div(squarenum,squareroot);

	return MAX*squareroot*divId.quot + divId.rem*squareroot + divSquareNum.quot*MAX + divSquareNum.rem;
}