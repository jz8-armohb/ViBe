#include <iostream>
#include "declarations.h"
using namespace std;

/************************************
* 8-Neighbour
* 0 1 2
* 3 8 4
* 5 6 7
*************************************/

int RandNumTo8NeighIdx(int randNum, int i, int j) {
	switch (randNum) {
	case 0:	// Top-left
		return ((i - 1) * W + j - 1);
		break;
	case 1:	// Top
		return ((i - 1) * W + j);
		break;
	case 2:	// Top-right
		return ((i - 1) * W + j + 1);
		break;
	case 3:	// Left
		return (i * W + j - 1);
		break;
	case 4:	// Right
		return (i * W + j + 1);
		break;
	case 5:	// Bottom-left
		return ((i + 1) * W + j - 1);
		break;
	case 6:	// Bottom
		return ((i + 1) * W + j);
		break;
	case 7:	// Bottom-right
		return ((i + 1) * W + j + 1);
		break;
	case 8:	// Current pixel
		return (i * W + j);
		break;
	default:
		break;
	}
}


int EdgePos(int i, int j) {
	int condition;
	if ((i == 0) && (j == 0)) {
		condition = 0;
	} else if ((i == 0) && (j == (W - 1))) {
		condition = 1;
	} else if ((i == (H - 1)) && (j == 0)) {
		condition = 2;
	} else if ((i == (H - 1)) && (j == (W - 1))) {
		condition = 3;
	} else if ((i == 0) && (j != 0) && (j != (W - 1))) {
		condition = 4;
	} else if ((i != 0) && (i != (H - 1)) && (j == 0)) {
		condition = 5;
	} else if ((i != 0) && (i != (H - 1)) && (j == (W - 1))) {
		condition = 6;
	} else {
		condition = 7;
	}
	//else if ((i == (H - 1)) && (j != 0) && (j != (W - 1))) {
	//	condition = 7;
	//}

	return condition;
}


int GetEdgeValidRand(int randNum, int condition) {
	switch (condition) {
	case 0:	// Top-left corner
		while ( (randNum != 4) && (randNum != 6) && (randNum != 7) && (randNum != 8) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 1:	// Top-right corner
		while ( (randNum != 3) && (randNum != 5) && (randNum != 6) && (randNum != 8) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 2:	// Bottom-left corner
		while ( (randNum != 1) && (randNum != 2) && (randNum != 4) && (randNum != 8) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 3:	// Bottom-right corner
		while ( (randNum != 0) && (randNum != 1) && (randNum != 3) && (randNum != 8) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 4:	// Top edge
		while ( (randNum == 5) || (randNum == 6) || (randNum == 7) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 5:	// Left edge
		while ( (randNum == 2) || (randNum == 4) || (randNum == 7) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 6:	// Right edge
		while ( (randNum == 0) || (randNum == 3) || (randNum == 5) ) {
			randNum = GetRandNum(8);
		}
		break;
	case 7:	// Bottom edge
		while ( (randNum == 0) || (randNum == 1) || (randNum == 2) ) {
			randNum = GetRandNum(8);
		}
		break;
	default:
		break;
	}

	return randNum;
}