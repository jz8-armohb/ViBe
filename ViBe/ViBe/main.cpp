#include <iostream>
#include <fstream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[]) {
	FILE* srcFilePtr;
	FILE* dstFilePtr;
	const char* srcFileName = "hall.yuv";
	const char* dstFileName = "hall_segmentationMap.yuv";
	int fileSize;	// Size of the video file
	int frameSize = W * H * 3 / 2;	// Size of one frame of the video
	int yFrameSize = W * H;
	int uvFrameSize = W * H / 2;
	int frameCnt;
	unsigned char* srcFrameYBuf = NULL;
	unsigned char* dstFrameUVBuf = NULL;

	
	/* Open the files */
	if (fopen_s(&srcFilePtr, srcFileName, "rb") == 0) {
		cout << "Successfully opened " << srcFileName << ".\n";
	} else {
		cout<<"WARNING!! Failed to open " << srcFileName << ".\n";
		exit(-1);
	}
	if (fopen_s(&dstFilePtr, dstFileName, "wb") == 0) {
		cout << "Successfully opened " << dstFileName << ".\n";
	} else {
		cout << "WARNING!! Failed to open " << dstFileName << ".\n";
		exit(-1);
	}

	/* Compute the size of the original file */
	fseek(srcFilePtr, 0L, SEEK_END);
	fileSize = ftell(srcFilePtr);
	rewind(srcFilePtr);
	cout << "Size of the orignal file: " << (double)fileSize / 1024 << " kB.\n";
	frameCnt = fileSize / frameSize;
	cout << "Frame count: " << frameCnt << ".\n";

	/* Space allocation & initialisation */
	srcFrameYBuf = new unsigned char[yFrameSize];
	dstFrameUVBuf = new unsigned char[uvFrameSize];
	memset(dstFrameUVBuf, 128, W * H / 4);


	/**************** ViBe ****************/
	/* Variables and buffers for ViBe */
	unsigned char image[W][H];	// Current frame
	unsigned char samples[W][H][N];	// Background model (samples of each pixel)
	unsigned char segMap[W][H];	// Background / foreground segmentation map
	unsigned char background = 0;	// Background identifier
	unsigned char foreground = 255;	// Foreground identifier

	/* Initialisation of backgroung model */
	fread(srcFrameYBuf, sizeof(unsigned char), yFrameSize, srcFilePtr);	// Read the first frame of the video (Y component) into the buffer
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ( (i == 0) || (i == (H - 1)) || (j == 0) || (j == (W - 1)) ) {
				for (int n = 0; n < N; n++) {
					int condition = EdgePos(i, j);
					int randNum = GetRandNum(8);
					randNum = GetEdgeValidRand(randNum, condition);
					int index = RandNumTo8NeighIdx(randNum, i, j);
					samples[i][j][n] = srcFrameYBuf[index];
				}
			} else {
				for (int n = 0; n < N; n++) {
					int randNum = GetRandNum(8);	// Get a random number with the range of 0 to 7
					int index = RandNumTo8NeighIdx(randNum, i, j);	// Compute the index of buffer of the 8-neighbour
					samples[i][j][n] = srcFrameYBuf[index];	// Store each sample
				}
			}
		}
	}
	fseek(srcFilePtr, uvFrameSize, SEEK_CUR);	// Skip chrominance components

	/* For each frame */
	for (int f = 1; f < frameCnt; f++) {
		fread(srcFrameYBuf, sizeof(unsigned char), W * H, srcFilePtr);
		fseek(srcFilePtr, uvFrameSize, SEEK_CUR);	// Skip chrominance components

		/* For each pixel */
		for (int i = 0; i < H; i++) {
			for (int j = 0; j < W; j++) {
				/* Compare each pixel to background model */
				int count = 0, index = 0, dist = 0;
				while ((count < SHARP_MIN) && (index < N)) {
					dist = abs(image[i][j] - samples[i][j][index]);	// Euclidean distance
					if (dist < R) {
						count++;
					}
					index++;
				}

				/* Classify pixel and update model */
				if (count >= SHARP_MIN) {
					segMap[i][j] = background;	// Mark the current pixel as background

					/* Update current pixel model */
					int randNum = GetRandNum(PHI);
					if (randNum == 0) {
						randNum = GetRandNum(N);	// Random subsampling
						samples[i][j][randNum] = image[i][j];	// Replace randomly chosen sample
					}

					/* Update neighbouring pixel model */
					randNum = GetRandNum(PHI);
					if (randNum == 0) {
						/* Random subsampling */
						randNum = GetRandNum(8);
						int index = RandNumTo8NeighIdx(randNum, i, j);
						int iNG = index / (int)W;
						int jNG = index % (int)W;

						randNum = GetRandNum(N);
						samples[iNG][jNG][randNum] = image[i][j];
					}

				} else {	/* count < SHARP_MIN */
					segMap[i][j] = foreground;	// Mark the current pixel as foreground
				}
			}
		}

		/* Write segmentation map into file */
		fwrite(segMap, sizeof(unsigned char), yFrameSize, dstFilePtr);
		fwrite(dstFrameUVBuf, sizeof(unsigned char), uvFrameSize, dstFilePtr);
	}


	/* CLose the files and delete the spaces */
	fclose(srcFilePtr);
	fclose(dstFilePtr);
	delete[]srcFrameYBuf;
	delete[]dstFrameUVBuf;
}