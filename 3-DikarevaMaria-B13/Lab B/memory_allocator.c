#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "memallocator.h"


void* memoryStart = NULL;
int memorySize = 0;
descriptor* head = NULL;
descriptor* curPtr = NULL;

int memgetminimumsize() {
    return sizeof(descriptor) + sizeof(int);
}

int memgetblocksize() {
   return sizeof(descriptor) + sizeof(int);
}

int* enddescriptor(descriptor* desc) {
    return (int*)((char*)desc + desc->size - sizeof(int));
}

int meminit(void* pMemory, int size) {
    if (size <= memgetminimumsize())
        return 0;
	memoryStart = (void*)pMemory;
    head = (descriptor*)pMemory;
    head->size = size;
    head->next = NULL;
    head->prev = NULL;
    int* endDescriptor = enddescriptor(head);
    *endDescriptor = size;
    curPtr = head;
    memorySize = size;

    return size;
}

void* memalloc(int size){
    if (size < 1 || size > memorySize)
        return NULL;
    descriptor* cur = curPtr;
    int* endDesc = NULL, * newEndDesc = NULL;
    if (cur != NULL)
        do {
            endDesc = enddescriptor(cur);
            if (cur->size - size > 2 * memgetblocksize()) {  
                //divide memory block to two blocks
                descriptor* newBlock = (descriptor*)((char*)cur + memgetblocksize() + size);
                newBlock->prev = cur->prev;
                newBlock->next = cur->next;
                newBlock->size = cur->size - size - memgetblocksize();
                if (cur->prev != NULL)
                    cur->prev->next = newBlock;
                else head = newBlock;
                if (cur->next != NULL)
                    cur->next->prev = newBlock;

                cur->size = memgetblocksize() + size;

                newEndDesc = enddescriptor(newBlock);
                *newEndDesc = newBlock->size;

                endDesc = enddescriptor(cur);
                *endDesc = -cur->size;
                curPtr = newBlock;

                return (void*)((char*)cur + sizeof(descriptor));
            }
            else { //give the whole block to user
                if (cur->size - memgetblocksize() >= size) {
                    endDesc = enddescriptor(cur);
                    *endDesc = -cur->size;

                    if (cur->prev != NULL)
                        cur->prev->next = cur->next;

                    if (head == cur)
                        head = cur->next;
                    if (cur->next != NULL) {
                        curPtr = cur->next;
                        cur->next->prev = cur->prev;
                    }
                    else curPtr = head;

                    return (void*)((char*)cur + sizeof(descriptor));
                }
            }
            cur = cur->next;
            if (cur == NULL)
                cur = head;
        } while (cur != curPtr);
    
    curPtr = head;
    return NULL;
}

void memfree(void* p) {
    descriptor* pdesc = (descriptor*)((char*)p - sizeof(descriptor));
    int* endDesc = enddescriptor(pdesc);
    int mergeLeftFlag = 0, mergeRightFlag = 0;

    *endDesc = pdesc->size;


    //merge left
    if ((char*)(pdesc - 1) > (char*)memoryStart) {
        int* leftBlockEndDesc = (int*)((char*)pdesc - sizeof(int));
        if (*leftBlockEndDesc > 0) {
            descriptor* leftBlockDesc = (descriptor*)((char*)pdesc - *leftBlockEndDesc);

            leftBlockDesc->size += pdesc->size;

            *enddescriptor(leftBlockDesc) = leftBlockDesc->size;
            pdesc = leftBlockDesc;

            if (curPtr == pdesc)
                curPtr = leftBlockDesc;

            mergeLeftFlag = 1;
        }
    }
   
    //merge right
    if ((char*)(endDesc + 1) < (char*)memoryStart + memorySize) {
        descriptor* rightBlockDesc = (descriptor*)((char*)pdesc + pdesc->size);
        int* rightBlockEndDesc = enddescriptor(rightBlockDesc);

        if (*rightBlockEndDesc > 0) {


            mergeRightFlag = 1;
            if (mergeLeftFlag == 0) {
                pdesc->next = head;
                head->prev = pdesc;
                pdesc->prev = NULL;
                head = pdesc;
            }

            if (rightBlockDesc == head) {
                head = rightBlockDesc->next;
                head->prev = NULL;
            }
            else {
                if  (rightBlockDesc->prev != NULL)
                    rightBlockDesc->prev->next = rightBlockDesc->next;
                if (rightBlockDesc->next != NULL)
                    rightBlockDesc->next->prev = rightBlockDesc->prev;
            }

            pdesc->size += rightBlockDesc->size;
            *enddescriptor(pdesc) = pdesc->size;
            if (curPtr == rightBlockDesc)
                curPtr = pdesc;

        }

    }

    if (mergeRightFlag == 0 && mergeLeftFlag == 0) {
        pdesc->prev = NULL;
        pdesc->next = head;
        if (head != NULL)
            head->prev = pdesc;
        head = pdesc;
        if (pdesc->next != NULL)
            pdesc->next->prev = pdesc;
    }
    if (curPtr == NULL)
        curPtr = pdesc;

}

void memdone() {
    descriptor* cur = (descriptor*)memoryStart;

    while ((char*)cur < (char*)memoryStart + memorySize) {
        if(*enddescriptor(cur) < 0)
            fprintf(stderr, "Memory leak detected! Block 0x%p\n", (char*)cur + sizeof(descriptor));
        cur = (descriptor*)((char*)cur + cur->size);
    }
}
