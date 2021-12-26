#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Node {
        int object;
        struct Node* next;
    } Node;

    typedef struct headList {
        Node* first;
    } headList;

    Node* ListInit();
    bool ListPush(Node** headlist, unsigned objNum);
    void ListPrint(Node* list, FILE* output);
    bool ReadInput(FILE* input);
    void ListDestroy(Node* list);
    long MaxVal(long a, long b);
    void DestroyTable(long** table);
    bool FillKnapsack(FILE* output);
    bool LabSolution(const char* filenameIn, const char* filenameOut);


#ifdef __cplusplus
}
#endif

#endif