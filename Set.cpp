#include "StdAfx.h"

Set::Set() {
	filled = 0;
}

void Set::Add(Square* square) {

	if (square != NULL && filled >= 0 && filled < MAX) {
		squares[filled] = square;
		filled++;
	}
	else
		MessageBox(NULL,"Error in Set::Add()","Sudoku",MB_OK);
}

void Set::RemoveLast() {

	if (filled > 0) {
		filled--;
		squares[filled] = NULL;
	}
	else
		MessageBox(NULL,"Error in Set::RemoveLast()","Sudoku",MB_OK);
	
}

Square* Set::GetSquare(int num) {

	if (num >= 0 && num < filled) 
		return squares[num];
	else {
		MessageBox(NULL,"Error in Set::GetSquare()","Sudoku",MB_OK);
		return NULL;
	}
}

Set Set::GetPossibilitiesFor(int number) {

	Set set = Set();
	
	for (int i = 0; i < filled; i++) {
		if ( squares[i]->CanHave(number) )
			set.Add(squares[i]);
	}

	return set;
}

int Set::Filled() {
	return filled;
}

// Locations match if all squares are also in toBeCompared set
bool Set::LocationsMatch(Set* toBeCompared) {

	if (toBeCompared == NULL) {
		MessageBox(NULL,"Error in Set::LocationsMatch()","Sudoku",MB_OK);
		return false;
	}

	if (filled == toBeCompared->Filled()) {
		for ( int i = 0; i < filled; i++ ) {
			if ( squares[i] != toBeCompared->GetSquare(i) )
				return false;
		}
		return true;
	}

	return false;
}

// Locations overlap if a square is also in toBeCompared set
bool Set::LocationsOverlap(Set* toBeCompared) {

	if (toBeCompared == NULL || toBeCompared == this) {
		MessageBox(NULL,"Error in Set::LocationsOverlap()","Sudoku",MB_OK);
		return false;
	}

	for ( int i = 0; i < filled; i++ ) {

		bool match = false;

		for (int y = 0; y < toBeCompared->Filled(); y++) {
			if ( squares[i] == toBeCompared->GetSquare(y) ) {
				match = true;
				break;
			}
		}
		if (!match)
			return false;

	}
	return true;
}

bool Set::Combine(Set* set) {
	
	if (set == NULL) {
		MessageBox(NULL,"Error in Set::Combine()","Sudoku",MB_OK);
	}

	int other_filled = set->Filled();
	for (int i = 0; i < other_filled; i++) {
		Square* square = set->GetSquare(i);
		if (!HasSquare(square))
			Add(square);
	}
	return true;
}

bool Set::HiddenMultiples(Set** tobeComparedList, int sets, Set* squaresMet) {

	if (tobeComparedList == NULL || squaresMet == NULL) {
		MessageBox(NULL,"Error in Set::HiddenMultiples()","Sudoku",MB_OK);
		return false;
	}

	if (filled < 2 || filled > sets+1)
		return false;

	for (int i = 0; i < sets; i++) {

		int other_filled = tobeComparedList[i]->Filled();
		if (other_filled < 2 || other_filled > sets+1)
			return false;

		squaresMet->Combine(tobeComparedList[i]);
	}

	squaresMet->Combine(this);

	if (squaresMet->Filled() != sets + 1)
		return false;

	return true;
}

bool Set::LocationsLine(Set* toBeCompared, bool HLine) {

	if (toBeCompared == NULL || toBeCompared == this) {
		MessageBox(NULL,"Error in Set::LocationsLine()","Sudoku",MB_OK);
		return false;
	}

	if (filled == toBeCompared->Filled()) {
		for ( int i = 0; i < filled; i++ ) {
			if ( HLine && squares[i]->HLine() != toBeCompared->GetSquare(i)->HLine() )
				return false;
			else if ( !HLine && squares[i]->VLine() != toBeCompared->GetSquare(i)->VLine() )
				return false;
		}
		return true;
	}

	return false;
}

bool Set::BlockAllExcluding(bool* matches) {

	bool makesDifference = false;

	for (int i = 0 ; i < MAX; i++) {
		if (!matches[i]) {
			for (int y = 0; y < filled; y++) {
				makesDifference = squares[y]->Block(i+1) || makesDifference;
			}
		}
	}
	return makesDifference;
}

bool Set::BlockAllExcluding(Set* set, int number) {

	if (set == NULL || set == this) {
		MessageBox(NULL,"Error in Set::BlockAllExcluding()","Sudoku",MB_OK);
		return false;
	}

	bool makesDifference = false;

	for (int i = 0; i < filled; i++) {

		if (!set->HasSquare(squares[i]))
			makesDifference = squares[i]->Block(number) || makesDifference;
	}

	return makesDifference;
}

bool Set::BlockAllOthers(bool* number_matches, Set* set) {

	if (set == NULL || set == this) {
		MessageBox(NULL,"Error in Set::BlockAllOthers()","Sudoku",MB_OK);
		return false;
	}

	bool makesDifference = false;

	for (int i = 0 ; i < set->Filled(); i++) {

		Square* square = set->GetSquare(i);

		if (!this->HasSquare(square)) {
			for (int y = 0 ; y < MAX; y++) {
				if (number_matches[y])
					makesDifference = square->Block(y+1) || makesDifference;
			}
		}
	}

	return makesDifference;
}

// Check if a number appears more than once in the set
bool Set::HasDuplicates() {

	for (int i = 0; i < filled; i++) {
		if (squares[i]->GetNumber() > 0) {
			for (int y = 0; y < filled; y++) {
				if (i != y && squares[i]->GetNumber() == squares[y]->GetNumber())
					return true;
			}
		}
	}

	return false;
}

bool Set::HasSquare(Square* square) {

	for (int i = 0; i < filled; i++) {
		if (squares[i] == square)
			return true;
	}

	return false;
}

bool Set::LinksWith(Set* set, bool vertically, Set* used) {

	if (set == NULL || set == this) {
		MessageBox(NULL,"Error in Set::LinksWith()","Sudoku",MB_OK);
		return false;
	}

	for (int i = 0; i < filled; i++) {
		if (!used->HasSquare(squares[i])) {

			for (int y = 0; y < set->Filled(); y++) {
				if (!used->HasSquare(set->GetSquare(y))) {

					if (( vertically && squares[i]->VLine() == set->GetSquare(y)->VLine()) ||
						(!vertically && squares[i]->HLine() == set->GetSquare(y)->HLine()) ) {
/*
						CString asd;
						asd.Format("banning: %d %d",squares[i]->GetID(),set->GetSquare(y)->GetID());
										MessageBox(NULL,asd,NULL,MB_OK);
*/
						used->Add(squares[i]);
						used->Add(set->GetSquare(y));
						return true;
					}
				}
			}
		}
	}

	return false;
}