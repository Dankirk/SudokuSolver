#include "StdAfx.h"

void (*Square::onBlockCallback)(int id, int number) = NULL;

Square::Square(int squareID) {

	id = squareID;
	Reset();
}

void Square::Reset() {

	assignedNumber = 0;

	for (int i = 0; i < MAX; i++)
		canHave[i] = true;
}

void Square::PlaceNum(int number) {

	if (number > 0 && number <= MAX) {

		assignedNumber = number;

		for(int i = 0; i < MAX; i++) {
			vLine->GetSquare(i)->Block(number);
			hLine->GetSquare(i)->Block(number);
			bigSquare->GetSquare(i)->Block(number);
		}

		for (int i = 1; i <= MAX; i++)
			Block(i);
	
	}
	else if (number != 0)
		MessageBox(NULL,"Error in Square::PlaceNum()","Sudoku",MB_OK);
}

bool Square::CanHave(int number) {

	if (number > 0 && number <= MAX)
		return canHave[number-1];
	else
		MessageBox(NULL,"Error in Square::CanHave()","Sudoku",MB_OK);
		
	return false;
}

void Square::CanHaveList(int (&list)[MAX]) {

	for (int num = 1; num <= MAX; num++) {
		if (CanHave(num))
			list[num-1] = 1;
		else
			list[num-1] = 0;
	}

}

bool Square::CanHaveAny(vector<int>* list) {
	
	for (unsigned int i = 0; i < list->size(); i++) {
		
		if (CanHave(list->at(i)))
			return true;
	}
	return false;
}

bool Square::Block(int number) {

	if (number > 0 && number <= MAX) {
		if (canHave[number-1]) {
			canHave[number-1] = false;

			if (onBlockCallback != NULL)
				(*onBlockCallback)(id, number);

			return true;
		}
	}
	else
		MessageBox(NULL,"Error in Square::Block()","Sudoku",MB_OK);

	return false;
}
int Square::CanHaveCount() {

	int total = 0;

	for (int i = 0; i < MAX; i++)
		if (canHave[i])
			total++;

	return total;
}

int Square::GetNumber() {
	return assignedNumber;
}

int Square::GetID() {
	return id;
}

Set* Square::VLine() {
	return vLine;
}
Set* Square::HLine() {
	return hLine;
}
Set* Square::BigSquare() {
	return bigSquare;
}

void Square::LinkVLine(Set* verLine) {
	vLine = verLine;
}

void Square::LinkHLine(Set* horLine) {
	hLine = horLine;
}

void Square::LinkBigSquare(Set* theBigSquare) {
	bigSquare = theBigSquare;
}