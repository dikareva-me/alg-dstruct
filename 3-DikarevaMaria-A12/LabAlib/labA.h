#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct XOR_list {
        char* str;
        struct XOR_list* link;

    } XOR_list;

    XOR_list* XOR(XOR_list* x, XOR_list* y);

    XOR_list* InitList(void);

    void AddToList(XOR_list** head, char* str);

    void Iteration(XOR_list** cur, XOR_list** prev);

    XOR_list* FindElement(XOR_list* head, XOR_list** prev, char* str);

    void DeleteElement(XOR_list* cur, XOR_list* prev);

    void DeleteElementKey(XOR_list* head, char* str);

    void DeleteElementAddress(XOR_list* head, XOR_list* ptr);

    void DestroyList(XOR_list* head);


#ifdef __cplusplus
}
#endif
