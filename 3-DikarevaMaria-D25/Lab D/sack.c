
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <stdbool.h>
#include "labd.h"

#pragma warning(disable:4996)

unsigned* g_objectWeight;
unsigned* g_objectCost;
unsigned g_objectCount;
unsigned g_sackCapacity;
unsigned g_requiredPrice;


Node* ListInit() {
    Node* list = (Node*)malloc(sizeof(Node));
    if (list == NULL)
        return NULL;
    list->next = NULL;
    list->object = 0;
    return list;
}

bool ListPush(Node** headlist, unsigned objNum) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
        return false;

    newNode->object = objNum;
    newNode->next = *headlist;
    *headlist = newNode;
    return true;
}

void ListPrint(Node* list, FILE* output) {
    Node* iter = list;
    while (iter != NULL) {
        fprintf(output, "%u ", iter->object);
        iter = iter->next;
    }
}

bool ReadInput(FILE* input) {
    fscanf(input, "%u %u %u", &g_objectCount, &g_sackCapacity, &g_requiredPrice);

    g_objectWeight = (unsigned*)malloc(g_objectCount * sizeof(unsigned));
    if (g_objectWeight == NULL) {
        return false;
    }
    g_objectCost = (unsigned*)malloc(g_objectCount * sizeof(unsigned));
    if (g_objectCost == NULL) {
        free(g_objectWeight);
        return false;
    }

    for (unsigned i = 0; i < g_objectCount; i++)
        fscanf(input, "%u ", &g_objectWeight[i]);
    for (unsigned i = 0; i < g_objectCount; i++)
        fscanf(input, "%u ", &g_objectCost[i]);

    return true;
}

void ListDestroy(Node* list) {
    Node* next = list;
    while (next != NULL) {
        Node* nodeToFree = next;
        next = next->next;
        free(nodeToFree);
    }
}

unsigned MaxVal(unsigned a, unsigned b) {
    return (a > b) ? a : b; 
}

void DestroyTable(unsigned** table) {
    for (unsigned i = 0; i < g_objectCount + 1; i++) {
        free(table[i]);
    }
    free(table);
}

bool FillKnapsack(FILE* output)
{
    unsigned weight, resultPrice;

    unsigned** tableSack = (unsigned**)malloc((g_objectCount + 1) * sizeof(unsigned*));
    if (tableSack == NULL) {
        free(g_objectWeight);
        free(g_objectCost);
        return false;
    }

    for (unsigned i = 0; i < g_objectCount + 1; i++) {
        tableSack[i] = (unsigned*)malloc((g_sackCapacity + 1) * sizeof(unsigned));
        if (tableSack[i] == NULL) {
            for (unsigned j = 0; j <= i; j++)
                free(tableSack[j]);
            free(tableSack);
            free(g_objectWeight);
            free(g_objectCost);
            return false;
        }
    }

    Node* foundItems = ListInit();
    if (foundItems == NULL) {
        DestroyTable(tableSack);
        free(g_objectWeight);
        free(g_objectCost);
        return false;
    }

    for (unsigned i = 0; i < g_objectCount + 1; i++) {
        for (unsigned w = 0; w < g_sackCapacity + 1; w++) {
            if (i == 0 || w == 0)
                tableSack[i][w] = 0;
            else
                if (g_objectWeight[i - 1] <= w)
                    tableSack[i][w] = MaxVal(g_objectCost[i - 1] +
                        tableSack[i - 1][w - g_objectWeight[i - 1]], tableSack[i - 1][w]);
                else
                    tableSack[i][w] = tableSack[i - 1][w];
        }
    }

    resultPrice = tableSack[g_objectCount][g_sackCapacity];

    if (resultPrice >= g_requiredPrice)
    {
        weight = g_sackCapacity;
        for (unsigned i = g_objectCount; i > 0 && resultPrice > 0; i--) {
            if (resultPrice == tableSack[i - 1][weight])
                continue;
            else {
                if (foundItems->object == 0)
                    foundItems->object = i;
                else
                    if (ListPush(&foundItems, i) == false) {
                        ListDestroy(foundItems);
                        DestroyTable(tableSack);
                        free(g_objectWeight);
                        free(g_objectCost);
                        return false;
                    }
                resultPrice = resultPrice - g_objectCost[i - 1];
                weight = weight - g_objectWeight[i - 1];
            }
        }
        ListPrint(foundItems, output);
    }
    else 
        fprintf(output, "0");

    ListDestroy(foundItems);
    DestroyTable(tableSack);
    free(g_objectWeight);
    free(g_objectCost);
    return true;
}

bool LabSolution(const char* filenameIn, const char* filenameOut) {
    FILE* fileIn = fopen(filenameIn, "r");
    if (fileIn == NULL)
        return false;
    FILE* fileOut = fopen(filenameOut, "w");
    if (fileOut == NULL)
        return false;

    if (ReadInput(fileIn) == 0) {
        printf("Failed to read file.");
        fclose(fileIn);
        fclose(fileOut);
        return false;
    }
    
    if (FillKnapsack(fileOut) == 0) {
        printf("Failed to run solution.");
        fclose(fileIn);
        fclose(fileOut);
        return false;
    }

    fclose(fileIn);
    fclose(fileOut);
    return true;
}
