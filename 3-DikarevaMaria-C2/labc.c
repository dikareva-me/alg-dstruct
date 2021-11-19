
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "labc.h"
#pragma warning(disable:4996)


Stack* StackInit() {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL)
        return NULL;
    stack->top = NULL;
    return stack;
}

Stack* StackPush(Stack* stack, int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
        return NULL;

    newNode->data = data;
    newNode->next = stack->top;
    stack->top = newNode;
    return stack;
}

int StackPop(Stack* stack) {
    if (StackEmpty(stack) == FALSE) {
        Node* nodeToFree = stack->top;
        int data = nodeToFree->data;
        stack->top = stack->top->next;
        free(nodeToFree);
        return data;
    }
    return EMPTY_STACK;
}

int StackEmpty(Stack* stack) {
    if (stack->top == NULL)
        return TRUE;
    return FALSE;
}

void StackDestroy(Stack* stack) {
    Node* next = stack->top;
    while (next != NULL) {
        Node* nodeToFree = next;
        next = next->next;
        free(nodeToFree);
    }
    free(stack);
}

void ReadGraph(FILE* stream, int** matrix) {
    char buf[MAX_SIZE];
    char* fgets_result = NULL;
    int vertex1, vertex2;

    fgets_result = fgets(buf, MAX_SIZE, stream);
    while (fgets_result != NULL) {
        sscanf(buf, "%d %d", &vertex1, &vertex2);
        matrix[vertex1][vertex2] = 1;
        matrix[vertex2][vertex1] = 1;
        fgets_result = fgets(buf, MAX_SIZE, stream);
    }
}

int** InitMatrix(int vertexNum) {
    int** matrix = (int**)malloc(sizeof(int*) * vertexNum);
    if (matrix == NULL)
        return NULL;

    for (int i = 0; i < vertexNum; i++) {
        matrix[i] = (int*)malloc(sizeof(int) * vertexNum);
        if (matrix[i] == NULL) {
            for (int j = 0; j <= i; j++)
                free(matrix[j]);
            free(matrix);
            return NULL;
        }
    }
    return matrix;
}

void FillZero(int** matrix, int vertexNum) {
    for (int i = 0; i < vertexNum; i++)
        for (int j = 0; j < vertexNum; j++)
            matrix[i][j] = 0;
}

void DestroyMatrix(int** matrix, int vertexNum) {
    for (int i = 0; i < vertexNum; i++)
        free(matrix[i]);
    free(matrix);
}

int DepthFirstSearch(FILE* stream, int** matrix, int vertexNum) {
    int* used = (int*)malloc(sizeof(int) * vertexNum);
    if (used == NULL)
        return FALSE;

    Stack* tmp = NULL;
    Stack* stack = StackInit();
    if (stack == NULL) {
        free(used);
        return FALSE;
    }

    int firstVertex = 0;
    tmp = StackPush(stack, firstVertex);
    if (tmp == NULL) {
        StackDestroy(stack);
        free(used);
        return FALSE;
    }
    else
        stack = tmp;

    for (int i = 0; i < vertexNum; i++)
        used[i] = 0;

    used[0] = 1;
    fprintf(stream, "%d ", stack->top->data);
    while (StackEmpty(stack) == FALSE) {
        int nextVertex = -1;
        int curVertex = stack->top->data;
        for (int i = 0; i < vertexNum; i++) {
            if (matrix[curVertex][i] == 1 && used[i] == 0) {
                nextVertex = i;
                break;
            }
        }
        if (nextVertex == -1)
            StackPop(stack);
        else {
            used[nextVertex] = 1;
            fprintf(stream, "%d ", nextVertex);
            tmp = StackPush(stack, firstVertex);
            if (tmp == NULL) {
                StackDestroy(stack);
                free(used);
                return FALSE;
            }
            else
                stack = tmp;

        }
    }
    StackDestroy(stack);
    free(used);
    return TRUE;
}
