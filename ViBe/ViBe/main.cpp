#include <iostream>
#include "declarations.h"
using namespace std;

int main(int argc, char* argv[]) {
	FILE* inFilePtr;
	FILE* outFilePtr;
	const char* inFileName = "hall.yuv";
	const char* outFileName = "hall_extracted.yuv";
	int fileSize;	// Size of the video file
	int frameSize = W * H * 3 / 2;	// Size of one frame of the video
	int yFrameSize = frameSize * 2 / 3;
	int uvFrameSize = frameSize / 6;
	int frameNum;
	unsigned char* inFrameYBuf = NULL;
	unsigned char* outFrameYBuf = NULL;
	unsigned char* outFrameUVBuf = NULL;
	
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

	/* Space allocation */
	inFrameYBuf = new unsigned char[yFrameSize];
	outFrameYBuf = new unsigned char[yFrameSize];
	outFrameUVBuf = new unsigned char[uvFrameSize];

	/* For each frame */
	for (int i = 0; i < frameNum; i++) {
		fread(inFrameYBuf, sizeof(unsigned char), yFrameSize, inFilePtr);

		/* ViBe */
	}

	fclose(inFilePtr);
	fclose(outFilePtr);
	delete[]inFrameYBuf;
	delete[]outFrameYBuf;
	delete[]outFrameUVBuf;
}