#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[]) {
	FILE* inFilePtr;
	FILE* outFilePtr;
	const char* inFileName = "hall.yuv";
	const char* outFileName = "hall_segmentationMap.yuv";
	int fileSize;	// Size of the video file
	int frameSize = W * H * 3 / 2;	// Size of one frame of the video
	int yFrameSize = W * H;
	int uvFrameSize = W * H / 2;
	int frameNum;
	unsigned char* inFrameYBuf = NULL;
	unsigned char* outFrameYBuf = NULL;
	unsigned char* outFrameUVBuf = NULL;

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
	unsigned char image[W][H];	// Current frame
	unsigned char samples[W][H][N];	// Background model (samples of each pixel)
	unsigned char segMap[W][H];	// Background / foreground segmentation map
	unsigned char background = 0;	// Background identifier
	unsigned char foreground = 255;	// Foreground identifier


	
	/* Open the files */
	if (fopen_s(&inFilePtr, inFileName, "rb") == 0) {
		cout << "Successfully opened " << inFileName << ".\n";
	} else {
		cout<<"WARNING!! Failed to open " << inFileName << ".\n";
		exit(-1);
	}
	if (fopen_s(&outFilePtr, outFileName, "wb") == 0) {
		cout << "Successfully opened " << outFileName << ".\n";
	} else {
		cout << "WARNING!! Failed to open " << outFileName << ".\n";
		exit(-1);
	}

	/* Compute the size of the original file */
	fseek(inFilePtr, 0L, SEEK_END);
	fileSize = ftell(inFilePtr);
	rewind(inFilePtr);
	cout << "Size of the orignal file: " << (double)fileSize / 1024 << " kB.\n";
	frameNum = fileSize / frameSize;

	/* Space allocation & initialisation */
	inFrameYBuf = new unsigned char[yFrameSize];
	//outFrameYBuf = new unsigned char[yFrameSize];
	outFrameUVBuf = new unsigned char[uvFrameSize];
	memset(outFrameUVBuf, 128, W * H / 4);


	/**************** ViBe ****************/
	/* Initialisation of backgroung model */
	fread(inFrameYBuf, sizeof(unsigned char), yFrameSize, inFilePtr);	// Read the first frame of the video (Y component) into the buffer
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if ( (i == 0) || (i == (H - 1)) || (j == 0) || (j == (W - 1)) ) {
				// 待完善
			} else {
				for (int n = 0; n < N; n++) {
					int randNum = GetRandNum(8);	// Get a random number with the range of 0 to 7
					int index = RandNumTo8NeighIdx(randNum, i, j);	// Compute the index of buffer of the 8-neighbour
					samples[i][j][n] = inFrameYBuf[index];	// Store each sample
				}
			}
		}
	}

	/* For each frame */
	for (int f = 0; f < frameNum; f++) {
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

						if ((iNG == 0) || (iNG == (H - 1)) || (jNG == 0) || (jNG == (W - 1))) {
							// 待完善
						} else {
							/* Replace randomly chosen sample */
							randNum = GetRandNum(8);
							samples[iNG][jNG][randNum] = image[i][j];

						}
					}

				} else {	/* count < SHARP_MIN */
					segMap[i][j] = foreground;	// Mark the current pixel as foreground
				}
			}
		}

		/* Write segmentation map into file */
		fwrite(segMap, sizeof(unsigned char), W * H, outFilePtr);
		fwrite(outFrameUVBuf, sizeof(unsigned char), W * H / 4, outFilePtr);
		fwrite(outFrameUVBuf, sizeof(unsigned char), W * H / 4, outFilePtr);
	}





	/* CLose the files and delete the spaces */
	fclose(inFilePtr);
	fclose(outFilePtr);
	delete[]inFrameYBuf;
	//delete[]outFrameYBuf;
	delete[]outFrameUVBuf;
}