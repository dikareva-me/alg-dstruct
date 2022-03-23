#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> 
#pragma warning (disable:4996)
#define SIZE_BUFFER 16

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

#define NIL &sentinel      
tree_t sentinel = { NIL, NIL, 0, BLACK, 0 };
tree_t* root = NIL;

tree_t* FindNode(int data) {
    tree_t* cur = root;
    while (cur != NIL)
        if (data == cur->data)
            break;
        else {
            if (cur->data < data)
                cur = cur->right;
            else
                cur = cur->left;
        }
    return cur;
}

void RotateLeft(tree_t* a) {
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
}

void RotateRight(tree_t* a) {
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
}

void FixInsert(tree_t* node) {
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
                    RotateLeft(node);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                RotateRight(node->parent->parent);
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
                    RotateRight(node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                RotateLeft(node->parent->parent);
            }
        }
    }
    root->color = BLACK;
}


tree_t* Insert(int data) {
    tree_t* cur = root;
    tree_t* prev = NULL;
    tree_t* new_node = (tree_t*)malloc(sizeof(tree_t));
    if (new_node == NULL)
        return NULL;

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
    
    FixInsert(new_node);
    return new_node;
}


void FixDelete(tree_t* node) {
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            tree_t* another_node = node->parent->right;
            if (another_node->color == RED) {
                another_node->color = BLACK;
                node->parent->color = RED;
                RotateLeft(node->parent);
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
                    RotateRight(another_node);
                    another_node = node->parent->right;
                }
                another_node->color = node->parent->color;
                node->parent->color = BLACK;
                another_node->right->color = BLACK;
                RotateLeft(node->parent);
                node = root;
            }
        }
        else {
            tree_t* another_node = node->parent->left;
            if (another_node->color == RED) {
                another_node->color = BLACK;
                node->parent->color = RED;
                RotateRight(node->parent);
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
                    RotateLeft(another_node);
                    another_node = node->parent->left;
                }
                another_node->color = node->parent->color;
                node->parent->color = BLACK;
                another_node->left->color = BLACK;
                RotateRight(node->parent);
                node = root;
            }
        }
    }
    node->color = BLACK;
}


bool Delete(int data) {
    tree_t* node = FindNode(data);
    if (node == NIL || node == NULL)
        return false;

    tree_t* node_to_free;
    if (node->left == NIL || node->right == NIL) {
        node_to_free = node;
    }
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
        FixDelete(child);

    free(node_to_free);
    return true;
}

void DestroyTree(tree_t* node) {
    if (node == NIL)
        return;
    if (node->left != NIL)
        DestroyTree(node->left);
    if (node->right != NIL)
        DestroyTree(node->right);

    free(node);
}

int main() {
    int data;
    char func;
    while (scanf("%c", &func) >= 1) {
        if (func != 'q') {
            scanf("%i", &data);
        }
        switch (func) {
        case 'a':
            Insert(data);
            break;
        case 'r':
            Delete(data);
            break;
        case 'f':
            if (FindNode(data) != NIL)
                puts("yes");
            else
                puts("no");
            break;
        case 'q':
            DestroyTree(root);
            return 0;
        }
    }
    DestroyTree(root);
    return 0;
}
