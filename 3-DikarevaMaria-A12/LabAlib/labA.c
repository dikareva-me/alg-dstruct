#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include < string.h >
#include "labA.h"

#include <locale.h>


XOR_list* XOR(XOR_list* x, XOR_list* y)
{
    return (XOR_list*)((uintptr_t)(x) ^ (uintptr_t)(y));
}

XOR_list* InitList(void)
{
    XOR_list* head = (XOR_list*)malloc(sizeof(XOR_list));
    if (head == NULL)
        return NULL;
    head->link = NULL;

    return head;
}

//Adding elements at the head of the list now
void AddToList(XOR_list** head, char* str)
{
    XOR_list* tmp = (XOR_list*)malloc(sizeof(XOR_list));
    if (tmp == NULL)
        return NULL;

    tmp->str = str;

    tmp->link = *head;
    if ((*head) != NULL)
        (*head)->link = XOR((*head)->link, tmp);

    *head = tmp;
}

void Iteration(XOR_list** cur, XOR_list** prev)
{
    XOR_list* next;
    next = XOR(*prev, (*cur)->link);
    *prev = *cur;
    *cur = next;
}

XOR_list* FindElement(XOR_list* head, XOR_list** prev, char* str)
{
    XOR_list* cur = head;

    while (cur != NULL)
        if (cur->str == NULL)
            Iteration(&cur, prev);
        else
            if (strcmp(cur->str, str) != 0)
                Iteration(&cur, prev);
            else
                break;

    return cur;
}

void DeleteElement(XOR_list* cur, XOR_list* prev)
{
    XOR_list* next;
    next = XOR(cur->link, prev);

    if (prev != NULL)
        prev->link = XOR(XOR(prev->link, cur), next);
    if (next != NULL)
        next->link = XOR(XOR(next->link, cur), prev);

    free(cur);
}

void DeleteElementKey(XOR_list* head, char* str)
{
    XOR_list* cur = head;
    XOR_list* prev = NULL;

    cur = FindElement(cur, &prev, str);

    if (cur != NULL)
        DeleteElement(cur, prev);
}

void DeleteElementAddress(XOR_list* head, XOR_list* ptr)
{
    XOR_list* cur = head;
    XOR_list* prev = NULL;

    while (cur != ptr)
        Iteration(&cur, &prev);

    if (cur != NULL)
        DeleteElement(cur, prev);
}

void DestroyList(XOR_list* head)
{
    XOR_list* cur;
    XOR_list* tmp;

    cur = head->link;

    while (cur != NULL)
    {
        tmp = head;
        Iteration(&cur, &head);
        free(tmp);
    }
    free(head);
}