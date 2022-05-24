#include "brenthash.h"
#pragma warning (disable:4996)


hashMap_t* initHashMap(int maxSize) {
    hashMap_t* map = (hashMap_t*)malloc(sizeof(hashMap_t));
    if (map) {
        map->data = (mapData_t*)malloc(sizeof(mapData_t) * maxSize);
        if (!map->data) {
            free(map);
            return NULL;
        }
        for (int i = 0; i < maxSize; i++) {
            map->data[i].value = NULL;
            map->data[i].state = HASH_FREE;
        }
        map->maxSize = maxSize;
        map->size = 0;
    }
    return map;
}

int H1func(const char* str, int maxSize) {
    const int c = 13;
    int hash = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        hash = hash * c + str[i];

    return hash % maxSize;
}

int H2func(const char* str, int maxSize) {
    const int c = 13;
    int hash = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++)
        hash = hash * c + str[i];

    return 1 + hash % (maxSize - 2);
}


void deleteHashMap(hashMap_t* map) {
    for (int i = 0; i < map->maxSize; i++)
        if (map->data[i].value)
            free(map->data[i].value);

    free(map->data);
    free(map);
}


int findIndex(const hashMap_t* map, const char* str) {
    int pos = H1func(str, map->maxSize);
    int step = H2func(str, map->maxSize);
    const int init_pos = pos;
    if (map->data[pos].state == HASH_FREE)
        return map->maxSize;

    while (map->data[pos].state != HASH_FREE &&
        (!map->data[pos].value || strcmp(map->data[pos].value, str))) {
        pos = (pos + step) % map->maxSize;
        if (pos == init_pos || map->data[pos].state == HASH_FREE)
            return map->maxSize;
    }
    return pos;
}

bool findData(const hashMap_t* map, const char* str) {
    return findIndex(map, str) != map->maxSize;
}

void deleteData(hashMap_t* map, const char* str) {
    int index = findIndex(map, str);
    if (index != map->maxSize) {
        map->data[index].state = HASH_DELETED;
        free(map->data[index].value);
        map->data[index].value = NULL;
    }
}

bool insertRightPos(hashMap_t* map, const char* str, int r) {
    int pos = H1func(str, map->maxSize);
    int step = H2func(str, map->maxSize);
    for (int i = 0; i < r - 1; i++) {
        int posI = (pos + i * step) % map->maxSize;
        int c = H2func(map->data[posI].value, map->maxSize);
        int k = r - i - 1;
        int posIK = (posI + k * c) % map->maxSize;
        if (map->data[posIK].state != HASH_FULL) {
            map->data[posIK] = map->data[posI];
            char* copy = (char*)malloc((strlen(str) + 1) * sizeof(char));
            if (!copy)
                return false;

            strcpy(copy, str);
            map->data[posI].value = copy;
            map->size++;
            return true;
        }
    }
    return false;
}

bool brentHashInsert(hashMap_t* map, const char* str) {
    if (map->size == map->maxSize)
        return false;
    if (findData(map, str))
        return true;

    int pos = H1func(str, map->maxSize);
    int step = H2func(str, map->maxSize);
    int r = 0;
    while (map->data[pos].state == HASH_FULL) {
        r++;
        pos = (pos + step) % map->maxSize;
        if (r >= 2)
            if (insertRightPos(map, str, r))
                return true;
    }
    char* copy = (char*)malloc((strlen(str) + 1) * sizeof(char));
    if (!copy)
        return false;

    strcpy(copy, str);
    map->data[pos].value = copy;
    map->data[pos].state = HASH_FULL;
    map->size++;
    return true;
}


bool LabSol(FILE* fileIn, FILE* fileOut) {
    hashMap_t* map = initHashMap(HASH_SIZE_DEFAULT);
    if (map == NULL)
        return false;

    char func;
    char key[20];
    while (fscanf(fileIn, "%c %s\n", &func, key) > 0) {
        switch (func) {
        case 'a': {
            brentHashInsert(map, key);
            break;
        }
        case 'r': {
            deleteData(map, key);
            break;
        }
        case 'f': {
            fprintf(fileOut, "%s ", findData(map, key) ? "yes" : "no");
            break;
        }
        default: {
            deleteHashMap(map);
            return true;
        }
        }
    }
    deleteHashMap(map);
    return true;
}