#include "pch.h"

#include "labc.h"
#include <crtdbg.h>
#include <Windows.h>

/*
IDE: Visual Studio 2019 Community Edition
OS: Windows 10 Pro Ver. 20H2 Build 19042.1288
PC configuration:
	CPU:            AMD Athlon 300U with Radeon Vega Mobile Gfx       2.40 GHz
	Motherboard:    HP 85EC 51.17
	RAM:            8.00 GB
	SSD:            477 GB

	 Time:  1.8 sec to generate graph
            21.136 sec to output answer
    RAM:    ~1.46 GB
*/


#define MAX_VERTIX 20000
#define NUM_EDGES 50000
#define MAX_INT_LENGTH 10

TEST(StressTest, StressTest_GraphGenerating) {
	const int maxVertex = MAX_VERTIX;
	FILE* file = fopen("file.txt", "w");
	ASSERT_TRUE(file);
	int vertex1 = 0, vertex2 = 0;

	fprintf(file, "%d\n", MAX_VERTIX);
	for (int i = 0; i < NUM_EDGES; i++) {
		do
			vertex2 = rand() % maxVertex;
		while (vertex2 == vertex1);
		fprintf(file, "%d %d\n", vertex1, vertex2);
		vertex1 = rand() % maxVertex;
	}
	fclose(file);
}

TEST(StressTest, StressTest_FillingInFile) {
	int depthFirstTraversalResult;
	FILE* fileIn = fopen("file.txt", "r");
	ASSERT_TRUE(fileIn);
	char* verticesCountBuf = (char*)malloc(MAX_INT_LENGTH * sizeof(char));
	char* check = fgets((char*)verticesCountBuf, MAX_INT_LENGTH, fileIn);
	if (check == NULL) {
		fclose(fileIn);
		free(verticesCountBuf);
		ASSERT_TRUE(check);
	}
	FILE* fileOut = fopen("result.txt", "w");
	if (fileOut == NULL) {
		fclose(fileIn);
		free(verticesCountBuf);
		ASSERT_TRUE(fileOut);
	}
	int numVertix = atoi((char*)verticesCountBuf);
	free(verticesCountBuf);
	int** matrix = InitMatrix(numVertix);
	if (matrix == NULL) {
		fclose(fileIn);
		fclose(fileOut);
		ASSERT_TRUE(matrix);
	}
	FillZero(matrix, numVertix);
	ReadGraph(fileIn, matrix);
	DepthFirstSearch(fileOut, matrix, numVertix);
	DestroyMatrix(matrix, numVertix);
	fclose(fileIn);
	fclose(fileOut);
}