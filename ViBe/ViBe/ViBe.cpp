#include <iostream>
#include "declarations.h"
using namespace std;

//#define N 20	// Number of samples per pixel
//#define R 20	// Radius of the sphere
//#define sharp_min 2	// Number of close samples for being part of the background
//#define PHI	16	// Amount of ramdom subsampling

void ViBe(unsigned char inFrame) {
	//int N = 20;	// Number of samples per pixel
	//int R = 20;	// Radius of the sphere
	//int sharp_min = 2;	// Number of close samples for being part of the background
	//int phi = 16;	// Amount of ramdom subsampling

	//unsigned char image[W][H];	// Current frame
	//struct SpatialNeighbour8 {
	//	unsigned char topL;
	//	unsigned char top;
	//	unsigned char topR;
	//	unsigned char left;
	//	unsigned char right;
	//	unsigned char botL;
	//	unsigned char bot;
	//	unsigned char botR;
	//}px8Neibour[W][H];	// 8-Neighbour of each pixel
	//unsigned char samples[W][H][N];	// Background model (samples of each pixel)
	//unsigned char segMap[W][H];	// Background / foreground segmentation map
	//unsigned char background = 0;	// Background identifier
	//unsigned char foreground = 255;	// Foreground identifier




	/* Initialisation of the bg model */
}

int RandNumTo8NeighIdx(int randNum, int i, int j) {
	switch (randNum) {
	case 0:	// Top-left
		return (i * (W - 1) + j - 1);
		break;
	case 1:	// Top
		return (i * (W - 1) + j);
		break;
	case 2:	// Top-right
		return (i * (W - 1) + j + 1);
		break;
	case 3:	// Left
		return (i * W + j - 1);
		break;
	case 4:	// Right
		return (i * W + j + 1);
		break;
	case 5:	// Bottom-left
		return (i * (W + 1) + j - 1);
		break;
	case 6:	// Bottom
		return (i * (W + 1) + j);
		break;
	case 7:	// Bottom-right
		return (i * (W + 1) + j + 1);
		break;
	default:
		break;
	}
}