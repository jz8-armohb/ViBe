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
	cout << "Size of the source file: " << (double)fileSize / 1024 << " kB.\n";
	frameCnt = fileSize / frameSize;
	cout << "Frame count: " << frameCnt << ".\n";

	/* Space allocation & initialisation */
	srcFrameYBuf = new unsigned char[yFrameSize];
	dstFrameUVBuf = new unsigned char[uvFrameSize];
	memset(dstFrameUVBuf, 128, uvFrameSize);


	/**************** ViBe ****************/
	/* Buffers for ViBe */
	unsigned char image[W][H];	// Current frame
	unsigned char* samples = new unsigned char[W * H * N];	// Background model (samples of each pixel)
	unsigned char* segMap = new unsigned char[W * H];	// Background / foreground segmentation map

	/* Initialisation of backgroung model */
	fread(srcFrameYBuf, sizeof(unsigned char), yFrameSize, srcFilePtr);	// Read the first frame of the video (Y component) into the buffer
	ViBe_Init(srcFrameYBuf, samples);
	fseek(srcFilePtr, uvFrameSize, SEEK_CUR);	// Skip chrominance components

	/* For each frame */
	for (int f = 1; f < frameCnt; f++) {
		fread(srcFrameYBuf, sizeof(unsigned char), W * H, srcFilePtr);
		fseek(srcFilePtr, uvFrameSize, SEEK_CUR);	// Skip chrominance components

		Vibe_FrameProcess(srcFrameYBuf, samples, segMap);

		/* Write segmentation map into file */
		fwrite(segMap, sizeof(unsigned char), yFrameSize, dstFilePtr);
		fwrite(dstFrameUVBuf, sizeof(unsigned char), uvFrameSize, dstFilePtr);
	}


	/* CLose the files and delete the spaces */
	fclose(srcFilePtr);
	fclose(dstFilePtr);
	delete[]srcFrameYBuf;
	delete[]dstFrameUVBuf;
	delete[]samples;
	delete[]segMap;
}