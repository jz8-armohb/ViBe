#pragma once
#define W 352
#define H 288
#define N 20	// Number of samples per pixel
#define R 20	// Radius of the sphere
#define SHARP_MIN 2	// Number of close samples for being part of the background
#define PHI	16	// Amount of ramdom subsampling
#define BACK 0
#define FORE 255
#define GetRandNum(x) (rand() % x)	// Generate a random number in a certain range


int RandNumTo8NeighIdx(int randNum, int i, int j);
int EdgePos(int i, int j);
int GetEdgeValidRand(int randNum, int condition);
void ViBe_Init(unsigned char* srcFrameYBuf, unsigned char* samples);
void Vibe_FrameProcess(unsigned char* srcFrameYBuf, unsigned char* samples, unsigned char* segMap);