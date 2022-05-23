#ifndef __HEADER_H_
#define __HEADER_H_

#define HASH_SIZE_DEFAULT 4000037
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum {
        HASH_FREE,
        HASH_FULL,
        HASH_DELETED
    } status;

    typedef struct record {
        char* value;
        status state;
    } mapData_t;

    typedef struct {
        mapData_t* data;
        int size;
        int maxSize;
    } hashMap_t;

    hashMap_t* initHashMap(int maxSize);
    int H1func(const char* str, int maxSize);
    int H2func(const char* str, int maxSize);
    void deleteHashMap(hashMap_t* map);
    int findIndex(const hashMap_t* map, const char* str);
    bool findData(const hashMap_t* map, const char* str);
    void deleteData(hashMap_t* map, const char* str);
    bool insertRightPos(hashMap_t* map, const char* str, int r);
    bool brentHashInsert(hashMap_t* map, const char* str);
    bool LabSol(FILE* fileIn, FILE* fileOut);


#ifdef __cplusplus
}
#endif

#endif