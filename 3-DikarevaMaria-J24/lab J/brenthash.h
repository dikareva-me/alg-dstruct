#ifndef HASH_H_
#define HASH_H_

#define HASH_SIZE_DEFAULT 500113

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
bool insertIRightPos(hashMap_t* map, const char* str, int r);
bool brentHashInsert(hashMap_t* map, const char* str);


#endif