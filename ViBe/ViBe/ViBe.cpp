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

void ViBe_Init(unsigned char* srcFrameYBuf, unsigned char* samples) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ((i == 0) || (i == (H - 1)) || (j == 0) || (j == (W - 1))) {
				for (int n = 0; n < N; n++) {
					int condition = EdgePos(i, j);
					int randNum = GetRandNum(9);
					randNum = GetEdgeValidRand(randNum, condition);
					int index = RandNumTo8NeighIdx(randNum, i, j);
					samples[i * W * N + j * N + n] = srcFrameYBuf[index];
				}
			} else {
				for (int n = 0; n < N; n++) {
					int randNum = GetRandNum(9);	// Get a random number with the range of 0 to 7
					int index = RandNumTo8NeighIdx(randNum, i, j);	// Compute the index of buffer of the 8-neighbour
					samples[i * W * N + j * N + n] = srcFrameYBuf[index];	// Store each sample
				}
			}
		}
	}
}

void Vibe_FrameProcess(unsigned char* srcFrameYBuf, unsigned char* samples, unsigned char* segMap) {
	/* For each pixel */
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			/* Compare each pixel to background model */
			int count = 0, index = 0, dist = 0;
			while ((count < SHARP_MIN) && (index < N)) {
				dist = abs((int)srcFrameYBuf[i * W + j] - (int)samples[i * W * N + j * N + index]);	// Euclidean distance
				if (dist < R) {
					count++;
				}
				index++;
			}

			/* Classify pixel and update model */
			if (count >= SHARP_MIN) {
				segMap[i * W + j] = BACK;	// Mark the current pixel as background

				/* Update current pixel model */
				int randNum = GetRandNum(PHI);
				if (randNum == 0) {
					randNum = GetRandNum(N);	// Random subsampling
					samples[i * W * N + j * N + randNum] = srcFrameYBuf[i * W + j];	// Replace randomly chosen sample
				}

				/* Update neighbouring pixel model */
				randNum = GetRandNum(PHI);
				if (randNum == 0) {
					/* Random subsampling */
					int randX = 0, randY = 0;
					while ((randX != 0) || (randY != 0)) {
						randX = GetRandNum(3) - 1;	// Get a random number from -1 and 1
						randY = GetRandNum(3) - 1;
					}
					randNum = GetRandNum(N);
					samples[(i + randY) * W * N + (j + randX) * N + randNum] = srcFrameYBuf[i * W + j];
					//randNum = GetRandNum(8);
					//int index = RandNumTo8NeighIdx(randNum, i, j);
					//int iNG = index / (int)W;
					//int jNG = index % (int)W;

					//randNum = GetRandNum(N);
					//samples[iNG * W * N + jNG * N + randNum] = srcFrameYBuf[i * W + j];
				}

			} else {	/* count < SHARP_MIN */
				segMap[i * W + j] = FORE;	// Mark the current pixel as foreground
			}
		}
	}

}