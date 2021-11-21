#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>

#define MAX_SIZE 10
#define EMPTY_STACK -1
#define TRUE 1
#define FALSE 0

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Node {
		int data;
		struct Node* next;
	}Node;

	typedef struct Stack {
		Node* top;
	}Stack;

	Stack* StackInit();
	Stack* StackPush(Stack* stack, int data);
	int StackPop(Stack* stack);
	int StackEmpty(Stack* stack);
	void StackDestroy(Stack* stack);
	void ReadGraph(FILE* stream, int** matrix);
	int** InitMatrix(int vertexNum);
	void FillZero(int** matrix, int vertexNum);
	void DestroyMatrix(int** matrix, int vertexNum);
	int DepthFirstSearch(FILE* stream, int** matrix, int vertexNum);



#ifdef __cplusplus
}
#endif

#endif