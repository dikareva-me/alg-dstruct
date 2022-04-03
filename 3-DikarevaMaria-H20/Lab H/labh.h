#ifndef __HEADER_H_
#define __HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> 

#define NIL &sentinel

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum {
		BLACK, RED
	} node_color;

	typedef struct tree_t {
		struct tree_t* left;
		struct tree_t* right;
		struct tree_t* parent;
		node_color color;
		int data;
	} tree_t;


	tree_t* RotateRight(tree_t* root, tree_t* a);

	tree_t* FixInsert(tree_t* root, tree_t* node);
	tree_t* FixDelete(tree_t* root, tree_t* node);
	tree_t* Delete(tree_t* root, int data);
	void DestroyTree(tree_t* node);
	tree_t* Insert(tree_t* root, int data);
	tree_t* FindNode(tree_t* root, int data);
	tree_t* RotateLeft(tree_t* root, tree_t* a);
	bool LabSol(FILE* fileIn, FILE* fileOut);

#ifdef __cplusplus
}
#endif

#endif