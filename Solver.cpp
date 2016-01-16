
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

	// Tarkista onko mill‰‰n setill‰ duplicaatteja
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
    Pienien laatikoiden numerotaulukko
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


	Isojen laatikoiden numerotaulukko
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
// Jos ruudulla vain yksi mahdollinen numero, laita numero siihen
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
// Jos numerolla vain yksi mahdollinen paikka setiss‰, laita numero siihen
////////////////////////////
bool Solver::SinglePossibilityInSet() {

	if (easyMode)
		return false;

	for (int number = 1; number <= MAX; number++) {
		for (int set = 0; set < MAX*3; set++) {

			// Hae setiss‰ olevat mahdollisuudet numerolle
			Set mahdollisuudet = sets[set].GetPossibilitiesFor(number);

			if (!easyMode) {
				if (mahdollisuudet.Filled() == 1) {
					mahdollisuudet.GetSquare(0)->PlaceNum(number);
					update = true;

					if (drawNumCallback != NULL)
						(*drawNumCallback)(mahdollisuudet.GetSquare(0)->GetID(), number);	
				}
			}
		}
	}
	return update;
}

//////////////////////////////
// "Subgroup exclusion"
// Jos isossa laatikossa olevat kaikki mahdolliset paikat ovat samassa vaaka- tai pystyriviss‰,
// est‰ ko. numeroiden asettaminen ko. riville muissa isoissa ruuduissa	
//    JA
// Jos samalla vaaka- tai pystyrivill‰ olevat mahdollisuudet ovat kaikki samassa isossa laatikossa,
// est‰ ko. numeroiden asettaminen ko. ison laatikon muihin kohtiin.
//////////////////////////////
bool Solver::SubgroupExclusion() {

	for (int number = 1; number <= MAX; number++) {
		for (int set = 0; set < MAX*3; set++) {
			
			// Hae setiss‰ olevat mahdollisuudet numerolle
			Set mahdollisuudet = sets[set].GetPossibilitiesFor(number);

			if (mahdollisuudet.Filled() > 1 && mahdollisuudet.Filled() <= max) {
					
				// K‰sitelt‰v‰n‰ iso ruutu
				if (set < MAX*2) {

					// Varmista ett‰ kaikki mahdollisuudet ovat ison ruudun sis‰ll‰
					if ( mahdollisuudet.LocationsOverlap( mahdollisuudet.GetSquare(0)->BigSquare() ) )

						// Blockaa ruutuja
						update = mahdollisuudet.GetSquare(0)->BigSquare()->BlockAllExcluding(&mahdollisuudet,number) || update;	
				}

				// K‰sitelt‰v‰ vaaka- tai pystyrivi
				else if (set >= MAX*2) {

					if ( mahdollisuudet.LocationsOverlap( mahdollisuudet.GetSquare(0)->HLine() ) )
						update = mahdollisuudet.GetSquare(0)->HLine()->BlockAllExcluding( mahdollisuudet.GetSquare(0)->BigSquare(), number ) || update;

					else if ( mahdollisuudet.LocationsOverlap( mahdollisuudet.GetSquare(0)->VLine() ) )
						update = mahdollisuudet.GetSquare(0)->VLine()->BlockAllExcluding( mahdollisuudet.GetSquare(0)->BigSquare(), number ) || update;
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

		// Hae kaikkien numeroiden mahdollisuudet setiss‰
		Set mahdollisuudet[MAX];
		for (int number = 1; number <= MAX; number++)
			mahdollisuudet[number-1] = sets[set].GetPossibilitiesFor(number);
				
				
		for (int number1 = 0; number1 < MAX; number1++) {

			// Hae t‰sm‰‰v‰t ja p‰‰llekk‰iset numero kanditaatit
			int matched = 1;
			bool matches[MAX];
			matches[number1] = true;

			int overlapped = 1;
			bool overlaps[MAX];
			overlaps[number1] = true;

			for (int number2 = 0; number2 < MAX; number2++) {
				if (number1 != number2) {
					if ( mahdollisuudet[number1].LocationsMatch( &mahdollisuudet[number2] ) ) {
						matches[number2] = true;
						matched++;
					}
					else
						matches[number2] = false;

					if ( mahdollisuudet[number1].LocationsOverlap( &mahdollisuudet[number2] ) ) {
						overlaps[number2] = true;
						overlapped++;
					}
					else
						overlaps[number2] = false;

				}
			}

			// Mik‰li edell‰ mainittuja kanditaatteja lˆytyi, blockataan siihen kuultumattomat
			if (matched > 1 && matched == mahdollisuudet[number1].Filled()) {
				update = mahdollisuudet[number1].BlockAllExcluding(matches) || update;
			}

			if (overlapped > 1 && overlapped == mahdollisuudet[number1].Filled()) {

				bool noOthers = true;

				for (int i = 0; i < overlapped; i++) {
					if ( mahdollisuudet[number1].GetSquare(i)->CanHaveCount() != overlapped) {
						noOthers =  false;
						break;
					}
				}
				if (noOthers)
					update = mahdollisuudet[number1].BlockAllOthers(overlaps,&sets[set]) || update;
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

		// K‰yd‰‰n l‰pi vain vaaka- ja pystyriveille (ei isoille neliˆille)
		for (int i = 0; i <= 1; i++) {

			// Varataan tilaa ainoastaan riveille,
			// joilla on numerolle enemm‰n kuin 1 paikka ja v‰hemm‰n kuin 9 paikkaa (2-8)
			int NumberOfBondedSets[MAX-2];
			Set BondedSets[MAX-2][MAX];
			for (int y = 0; y < MAX-2; y++)
				NumberOfBondedSets[y] = 0;


			// Tehd‰‰n mahdollisten paikkojen mukaan taulukko,
			// josta voidaan etsi‰ onko mahdolliset ruudut samoilla vaaka- tai pystyriveill‰
			for (int set = 0; set < MAX; set++) {
				Set tmpSet = sets[i*MAX + set].GetPossibilitiesFor(number);
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
				if (NumberOfBondedSets[y] == y+2) {	// mahdollisuuksia rivill‰ yht‰ monta kuin rivej‰

					bool lineV;
					bool lineH;

					if (i == 0) {
						lineV = true;
						lineH = false;
					}
					else {
						lineV = false;
						lineH = true;
					}

					// Selvitet‰‰n kumman tyyppisen linjan ruudut muodostavat (jos muodostavat)
					for (int set = 1; set < NumberOfBondedSets[y]; set++) {
						if		(i == 0 && !BondedSets[y][0].LocationsLine(&BondedSets[y][set],false)) {
							lineV = false;
							break;
						}
						else if (i == 1 && !BondedSets[y][0].LocationsLine(&BondedSets[y][set],true )) {
							lineH = false;
							break;
						}
					} 

					if (lineV || lineH) {

						for (int square = 0; square < NumberOfBondedSets[y]; square++) {

							Set lineToBlock;
							if (lineV)
								lineToBlock = BondedSets[y][0].GetSquare(square)->VLine()->GetPossibilitiesFor(number);
							else
								lineToBlock = BondedSets[y][0].GetSquare(square)->HLine()->GetPossibilitiesFor(number);


							// Tarkistetaan onko rivill‰ blockattavaa
							if (lineToBlock.Filled() > NumberOfBondedSets[y]) {

								for (int bSquare = 0; bSquare < lineToBlock.Filled(); bSquare++) {

									// ƒl‰ blockkaa rivilt‰ ruutuja, jotka muodostivat X-Wingin
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
						
				bool vertically;

				if (i == 0)
					vertically = true;
				else
					vertically = false;
						
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

							Set used = Set();	// linkityksess‰ k‰ytetyt ruudut

							if ( sf_sets[set[0]]->LinksWith( sf_sets[set[1]], vertically, &used ) ) {
	
								for (set[2] = 0; set[2] < sf_setsNum; set[2]++) {

									if (set[2] != set[0] && 
										set[2] != set[1]) {

										Set used_tmp = used;

										if (sf_sets[set[2]]->LinksWith( sf_sets[set[0]], vertically, &used_tmp ) &&
											sf_sets[set[2]]->LinksWith( sf_sets[set[1]], vertically, &used_tmp ) ) {
												
											// tarkista ett‰ settien kaikki ruudut ovat mukana linkiss‰
											if (used_tmp.Filled() ==	sf_sets[set[0]]->Filled() + 
																		sf_sets[set[1]]->Filled() + 
																		sf_sets[set[2]]->Filled()) {
														
												// swordfish found, start restricting...

												for (int j = 0; j < 3; j++) {
													for (int k = 0; k < sf_sets[set[j]]->Filled(); k++) {

														if (vertically) {
															update = sf_sets[set[j]]->GetSquare(k)->VLine()->BlockAllExcluding(&used_tmp,number) || update;
														}
														else {
															update = sf_sets[set[j]]->GetSquare(k)->HLine()->BlockAllExcluding(&used_tmp,number) || update;
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

	// lis‰‰ ruutu ja sen numerot listaan
	used_squares->Add(square);

	for (int i = 1; i <= MAX; i++) {
		if (square->CanHave(i))
			used_numbers->push_back(i);
	}

	// laske eri numeroiden m‰‰r‰ used_numbers vektorissa
	int differentNumbers = 0;
	bool numbers[MAX];

	for (int i = 0; i < MAX; i++)
		numbers[i] = false;

	for (unsigned int i = 0; i < used_numbers->size(); i++) {
		int index = used_numbers->at(i)-1;
		if (!numbers[index]) {
			numbers[index] = true;
			differentNumbers++;
		}
	}

	// jos ruutuja on yht‰ monta kuin eri numeroita, naked linkki on lˆytynyt
	if (used_squares->Filled() == differentNumbers)
		return true;

	// lis‰‰ linkkiin jokin uusi ruutu, jossa on jokin jo setin numeroista
	else {
		bool ok = false;
		for (int i = 0; i < set->Filled(); i++) {
			Square* ruutu = set->GetSquare(i);
			if (!used_squares->HasSquare(ruutu)) {
				if (ruutu->CanHaveAny(used_numbers)) {
					if (FindNakedSetRecursive( set, used_numbers, used_squares, ruutu)) {
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