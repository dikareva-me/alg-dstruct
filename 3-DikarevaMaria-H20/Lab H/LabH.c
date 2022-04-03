#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> 
#include "labh.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#pragma warning (disable:4996)
#define SIZE_BUFFER 16

tree_t sentinel = { NIL, NIL, NULL, BLACK, 0 };
tree_t* FindNode(tree_t* root, int data) {
    tree_t* cur = root;
    while (cur != NIL && cur != NULL) {
        if (data == cur->data)
            break;
        else {
            if (cur->data < data)
                cur = cur->right;
            else
                cur = cur->left;
        }
    }
    return cur;
}

tree_t* RotateLeft(tree_t* root, tree_t* a) {
    tree_t* b = a->right;

    if (b != NIL)
        b->parent = a->parent;

    if (a->parent != NULL) {
        if (a->parent->left == a)
            a->parent->left = b;
        else
            a->parent->right = b;
    }
    else
        root = b;

    a->right = b->left;
    if (b->left != NIL)
        b->left->parent = a;

    b->left = a;
    if (a != NIL)
        a->parent = b;

    return root;
}

tree_t* RotateRight(tree_t* root, tree_t* a) {
    tree_t* b = a->left;

    if (b != NIL)
        b->parent = a->parent;

    if (a->parent != NULL) {
        if (a->parent->left == a)
            a->parent->left = b;
        else
            a->parent->right = b;
    }
    else
        root = b;

    a->left = b->right;
    if (b->right != NIL)
        b->right->parent = a;

    b->right = a;
    if (a != NIL)
        a->parent = b;

    return root;
}

tree_t* FixInsert(tree_t* root, tree_t* node) {
    while (node != root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            //node and it's parent are on the left, uncle on the right
            tree_t* another_node = node->parent->parent->right;
            if (another_node->color == RED) {
                //if uncle is red
                node->parent->color = BLACK;
                another_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                //if uncle is black
                if (node == node->parent->right) {
                    node = node->parent;
                    root = RotateLeft(root, node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = RotateRight(root, node->parent->parent);
            }
        }
        else {
            //node and it's parent are on the right, uncle on the left
            tree_t* another_node = node->parent->parent->left;
            if (another_node->color == RED) {
                node->parent->color = BLACK;
                another_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    root = RotateRight(root, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                root = RotateLeft(root, node->parent->parent);
            }
        }
    }
    (root)->color = BLACK;

    return root;
}


tree_t* Insert(tree_t* root, int data) {
    tree_t* cur = root;
    tree_t* prev = NULL;
    tree_t* new_node = (tree_t*)malloc(sizeof(tree_t));
    if (new_node == NULL)
        return root;

    while (cur != NIL) {
        if (data == cur->data)
            return cur;
        prev = cur;
        if (cur->data < data)
            cur = cur->right;
        else
            cur = cur->left;
    }
    new_node->parent = prev;
    new_node->data = data;
    new_node->left = NIL;
    new_node->right = NIL;
    new_node->color = RED;

    if (prev != NULL) {
        if (prev->data < data)
            prev->right = new_node;
        else
            prev->left = new_node;
    }
    else
        root = new_node;
    root = FixInsert(root, new_node);
    return root;
}


tree_t* FixDelete(tree_t* root, tree_t* node) {
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            tree_t* another_node = node->parent->right;
            if (another_node->color == RED) {
                another_node->color = BLACK;
                node->parent->color = RED;
                root = RotateLeft(root, node->parent);
                another_node = node->parent->right;
            }
            if (another_node->left->color == BLACK && another_node->right->color == BLACK) {
                another_node->color = RED;
                node = node->parent;
            }
            else {
                if (another_node->right->color == BLACK) {
                    another_node->left->color = BLACK;
                    another_node->color = RED;
                    root = RotateRight(root, another_node);
                    another_node = node->parent->right;
                }
                another_node->color = node->parent->color;
                node->parent->color = BLACK;
                another_node->right->color = BLACK;
                root = RotateLeft(root, node->parent);
                node = root;
            }
        }
        else {
            tree_t* another_node = node->parent->left;
            if (another_node->color == RED) {
                another_node->color = BLACK;
                node->parent->color = RED;
                root = RotateRight(root, node->parent);
                another_node = node->parent->left;
            }
            if (another_node->right->color == BLACK && another_node->left->color == BLACK) {
                another_node->color = RED;
                node = node->parent;
            }
            else {
                if (another_node->left->color == BLACK) {
                    another_node->right->color = BLACK;
                    another_node->color = RED;
                    root = RotateLeft(root, another_node);
                    another_node = node->parent->left;
                }
                another_node->color = node->parent->color;
                node->parent->color = BLACK;
                another_node->left->color = BLACK;
                root = RotateRight(root, node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
    return root;
}



tree_t* Delete(tree_t* root, int data) {
    tree_t* node = FindNode(root, data);
    if (node == NIL || node == NULL)
        return root;

    tree_t* node_to_free;
    if (node->left == NIL || node->right == NIL)
        node_to_free = node;
    else {
        node_to_free = node->right;
        while (node_to_free->left != NIL)
            node_to_free = node_to_free->left;
    }

    tree_t* child;
    if (node_to_free->left != NIL)
        child = node_to_free->left;
    else
        child = node_to_free->right;

    child->parent = node_to_free->parent;
    if (node_to_free->parent != NULL) {
        if (node_to_free == node_to_free->parent->left)
            node_to_free->parent->left = child;
        else
            node_to_free->parent->right = child;
    }
    else
        root = child;

    if (node_to_free != node)
        node->data = node_to_free->data;
    if (node_to_free->color == BLACK)
        root = FixDelete(root, child);

    free(node_to_free);
    return root;
}

void DestroyTree(tree_t* node) {
    if (node == NIL || node == NULL)
        return;
    if (node->left != NIL)
        DestroyTree(node->left);
    if (node->right != NIL)
        DestroyTree(node->right);

    free(node);
}

bool LabSol(FILE* fileIn, FILE* fileOut) {
    int data;
    char func;
    tree_t* root = NIL;
    tree_t* found_node = NULL;

    while (fscanf(fileIn, "%c", &func) >= 1) {
        if (func != 'q')
            fscanf(fileIn, "%i", &data);
        switch (func) {
        case 'a':
            root = Insert(root, data);
            break;
        case 'r':
            root = Delete(root, data);
            break;
        case 'f':
            found_node = FindNode(root, data);
            if (found_node != NIL && found_node != NULL)
                fprintf(fileOut, "yes ");
            else
                fprintf(fileOut, "no ");
            break;
        case 'q':
            DestroyTree(root);
            return 0;
        }
    }
    DestroyTree(root);
    return true;
}