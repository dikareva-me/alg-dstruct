#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable:4996)

typedef struct tree_t {
    int keys[2];
    int height;
    struct tree_t* sons[3];
    struct tree_t* parent;
    bool full;
} tree_t;

tree_t* InitNode(tree_t* brother, int key, tree_t* son) {
    tree_t* new_node = (tree_t*)malloc(sizeof(tree_t));
    if (new_node == NULL)
        return NULL;

    new_node->full = false;
    new_node->keys[0] = key;
    new_node->sons[0] = son;
    if (brother != NULL) {
        new_node->height = brother->height;
        if (new_node->height != 0)
            new_node->sons[0]->parent = new_node;
        new_node->parent = brother->parent;
        new_node->sons[2] = brother->sons[2];
        brother->sons[2] = new_node;
    }
    else {
        new_node->height = 0;
        new_node->parent = NULL;
        new_node->sons[2] = NULL;
    }
    return new_node;
}

tree_t* FindRoot(tree_t* node) {
    tree_t* cur = node;
    while (cur->parent) {
        cur = cur->parent;
    }
    return cur;
}

void DestroyTree(tree_t* tree) {
    if (tree != NULL) {
        if (tree->height != 0) {
            DestroyTree(tree->sons[0]);
            DestroyTree(tree->sons[1]);
            if (tree->full == true)
                DestroyTree(tree->sons[2]);
        }
        free(tree);
    }
}

void CorrectPlaceInsert(tree_t* node, int key, tree_t* son) {
    if (key > node->keys[0]) {
        node->keys[1] = key;
        node->sons[1] = son;
    }
    else {
        node->keys[1] = node->keys[0];
        node->sons[1] = node->sons[0];
        node->keys[0] = key;
        node->sons[0] = son;
    }
}

tree_t* FindNode(tree_t* tree, int key) {
    if (tree == NULL)
        return NULL;

    tree_t* cur = tree;
    while (cur->height != 0) {
        if (key <= cur->keys[0])
            cur = cur->sons[0];

        else
            if (cur->full == true && key > cur->keys[1])
                cur = cur->sons[2];
            else
                cur = cur->sons[1];

    }
    return cur;
}

tree_t* AboveInsert(tree_t* tree, int key) {
    tree_t* node_parent = tree->parent;
    if (node_parent == NULL) {
        node_parent = InitNode(NULL, key, tree);
        if (node_parent == NULL) {
            DestroyTree(tree->sons[2]);
            DestroyTree(tree);
            return NULL;
        }
        node_parent->height = tree->height + 1;
        node_parent->sons[1] = tree->sons[2];
        tree->sons[2] = NULL;

        tree->parent = node_parent;
        node_parent->sons[1]->parent = node_parent;
        return node_parent;
    }
    if (node_parent->full == 0) {
        if (key > node_parent->keys[0]) {
            node_parent->keys[1] = key;
            node_parent->sons[2] = tree->sons[2];
            node_parent->sons[2]->parent = node_parent;
            tree->sons[2] = NULL;
        }
        else {
            node_parent->keys[1] = node_parent->keys[0];
            node_parent->sons[2] = node_parent->sons[1];

            node_parent->keys[0] = key;
            node_parent->sons[1] = tree->sons[2];
            node_parent->sons[1]->parent = node_parent;
            tree->sons[2] = NULL;
        }
        node_parent->full = true;
    }
    else {
        if (key < node_parent->keys[0]) {
            tree_t* next_node = InitNode(node_parent, node_parent->keys[1], node_parent->sons[1]);
            if (next_node == NULL) {
                DestroyTree(tree->sons[2]);
                DestroyTree(FindRoot(node_parent));
                return NULL;
            }
            node_parent->full = false;
            node_parent->keys[1] = 0;

            next_node->sons[1] = next_node->sons[2];
            next_node->sons[2] = NULL;
            next_node->sons[1]->parent = next_node;

            int old_key = node_parent->keys[0];
            node_parent->keys[0] = key;
            node_parent->sons[1] = tree->sons[2];
            tree->sons[2] = NULL;

            return AboveInsert(node_parent, old_key);
        }
        else
            if (key < node_parent->keys[1]) {
                tree_t* next_node = InitNode(node_parent, node_parent->keys[1], tree->sons[2]);
                if (next_node == NULL) {
                    DestroyTree(tree->sons[2]);
                    DestroyTree(FindRoot(node_parent));
                    return NULL;
                }
                node_parent->full = false;
                node_parent->keys[1] = 0;

                next_node->sons[1] = next_node->sons[2];
                next_node->sons[1]->parent = next_node;
                next_node->sons[2] = NULL;
                tree->sons[2] = NULL;

                return AboveInsert(node_parent, key);
            }
            else {
                tree_t* next_node = InitNode(node_parent, key, node_parent->sons[2]);
                if (next_node == NULL) {
                    DestroyTree(tree->sons[2]);
                    DestroyTree(FindRoot(node_parent));
                    return NULL;
                }
                node_parent->full = false;
                int old_key = node_parent->keys[1];
                node_parent->keys[1] = 0;

                next_node->sons[1] = tree->sons[2];
                next_node->sons[2] = NULL;
                next_node->sons[1]->parent = next_node;
                tree->sons[2] = NULL;

                return AboveInsert(node_parent, old_key);
            }
    }
    return FindRoot(node_parent);
}

tree_t* InsertNode(tree_t* tree, int key, tree_t* son) {
    if (tree == NULL)
        return InitNode(NULL, key, son);

    tree_t* new_node = FindNode(tree, key);
    if (key == new_node->keys[0] || (new_node->full == true && key == new_node->keys[1]))
        return tree;

    if (new_node->full == false) {
        CorrectPlaceInsert(new_node, key, son);
        new_node->full = true;
        return tree;
    }
    else {
        if (key < new_node->keys[0]) {
            tree_t* next_node = InitNode(new_node, new_node->keys[1], new_node->sons[1]);
            if (next_node == NULL) {
                DestroyTree(tree);
                return NULL;
            }
            CorrectPlaceInsert(new_node, key, son);
            return AboveInsert(new_node, new_node->keys[1]);
        }
        else
            if (key < new_node->keys[1]) {
                tree_t* next_node = InitNode(new_node, new_node->keys[1], new_node->sons[1]);
                if (next_node == NULL) {
                    DestroyTree(tree);
                    return NULL;
                }
                CorrectPlaceInsert(new_node, key, son);
                return AboveInsert(new_node, key);
            }
            else {
                tree_t* next_node = InitNode(new_node, key, son);
                if (next_node == NULL) {
                    DestroyTree(tree);
                    return NULL;
                }
                return AboveInsert(new_node, new_node->keys[1]);
            }
    }
}


tree_t* Merge(tree_t* left, tree_t* right) {
    if (left == NULL)
        return right;
    if (right == NULL)
        return left;

    if (left->height == right->height) {
        tree_t* left_parent = left->parent;
        tree_t* right_parent = right->parent;
        int max_key;
        tree_t* cur = left;
        while (cur->height != 0)
            cur = (cur->full == false) ? cur->sons[1] : cur->sons[2];
        max_key = (cur->full == false) ? cur->keys[0] : cur->keys[1];

        if (left_parent != NULL) {
            right->parent = left_parent;
            if (left_parent->full == false) {
                left_parent->keys[1] = max_key;
                left_parent->sons[2] = right;
                left_parent->full = true;
                return FindRoot(left_parent);
            }
            else {
                tree_t* next_node = InitNode(left_parent, max_key, left);
                if (next_node == NULL)
                    return NULL;
                next_node->sons[1] = right;
                right->parent = next_node;
                next_node->sons[2] = NULL;
                left_parent->full = false;
                return AboveInsert(left_parent, left_parent->keys[1]);
            }
        }
        else
            if (right_parent != NULL) {
                left->parent = right_parent;
                if (right_parent->full == false) {
                    right_parent->keys[1] = right_parent->keys[0];
                    right_parent->sons[2] = right_parent->sons[1];
                    right_parent->sons[1] = right_parent->sons[0];

                    right_parent->keys[0] = max_key;
                    right_parent->sons[0] = left;
                    right_parent->full = true;
                    return FindRoot(right_parent);
                }
                else {
                    tree_t* next_node = InitNode(right_parent, right_parent->keys[1], right_parent->sons[1]);
                    if (next_node == NULL)
                        return NULL;
                    right_parent->full = false;
                    next_node->sons[1] = next_node->sons[2];
                    next_node->sons[1]->parent = next_node;
                    next_node->sons[2] = NULL;
                    int old_key = right_parent->keys[0];
                    right_parent->sons[1] = right_parent->sons[0];
                    right_parent->sons[0] = left;
                    right_parent->keys[0] = max_key;
                    return AboveInsert(right_parent, old_key);
                }
            }
            else {
                tree_t* tree = InitNode(NULL, max_key, left);
                if (tree == NULL)
                    return NULL;
                tree->height = left->height + 1;
                tree->sons[1] = right;
                left->parent = tree;
                right->parent = tree;
                return tree;
            }
    }
    if (left->height < right->height)
        return Merge(left, right->sons[0]);
    else
        return Merge((left->full == false) ? left->sons[1] : left->sons[2], right);
}

tree_t* TakeOut(tree_t** node) {
    tree_t* res = *node;
    *node = NULL;
    res->parent = NULL;
    return res;
}

tree_t* Split(tree_t* tree, int key, tree_t** node_left, tree_t** node_right) {
    if (tree != NULL) {
        if (tree->height != 0) {
            tree_t* left = TakeOut(&tree->sons[0]);
            tree_t* middle = TakeOut(&tree->sons[1]);
            tree_t* right = NULL;
            if (tree->full == true)
                right = TakeOut(&tree->sons[2]);

            if (key <= tree->keys[0]) {
                if (right != NULL)
                    *node_right = Merge(right, *node_right);
                *node_right = Merge(middle, *node_right);
                free(tree);
                return Split(left, key, node_left, node_right);
            }
            *node_left = Merge(*node_left, left);

            if (tree->full == true && key > tree->keys[1]) {
                *node_left = Merge(*node_left, middle);
                free(tree);
                return Split(right, key, node_left, node_right);
            }
            if (right != NULL)
                *node_right = Merge(right, *node_right);
            free(tree);

            return Split(middle, key, node_left, node_right);
        }
        if (key < tree->keys[0]) {
            *node_right = InsertNode(*node_right, tree->keys[0], tree->sons[0]);
            if (tree->full == true)
                *node_right = InsertNode(*node_right, tree->keys[1], tree->sons[1]);
            free(tree);
            return NULL;
        }
        if (key == tree->keys[0]) {
            if (tree->full == true)
                *node_right = InsertNode(*node_right, tree->keys[1], tree->sons[1]);
            tree->parent = NULL;
            tree->keys[1] = 0;
            tree->sons[1] = NULL;
            return tree;
        }
        *node_left = InsertNode(*node_left, tree->keys[0], tree->sons[0]);
        if (tree->full == true && key < tree->keys[1]) {
            *node_right = InsertNode(*node_right, tree->keys[1], tree->sons[1]);
            free(tree);
            return NULL;
        }
        if (tree->full == true && key == tree->keys[1]) {
            tree->parent = NULL;
            tree->keys[0] = key;
            tree->sons[0] = tree->sons[1];
            tree->keys[1] = 0;
            tree->sons[1] = NULL;
            tree->full = false;
            return tree;
        }
        if (tree->full == true)
            *node_left = InsertNode(*node_left, tree->keys[1], tree->sons[1]);
        free(tree);
        return NULL;
    }
    return NULL;
}

tree_t* DeleteNode(tree_t* tree, int key) {
    tree_t* left = NULL, * right = NULL;
    tree_t* node = FindNode(tree, key);

    if (node == NULL || (key != node->keys[0] && (node->full == true && key != node->keys[1])))
        return tree;

    tree_t* temp = Split(tree, key, &left, &right);
    free(temp);
    return Merge(left, right);
}


/*
New print looks like this. The arrow of the full node points at the first key.

          ->[ 1,  2]
    -> 2
          ->[ 3,  4]
 4
          ->[ 5,  6]
    -> 6
          ->[ 7,  8]
       8
          ->[ 9]

          */
void PrintTree(tree_t* tree, int offset) {
    if (tree == NULL)
        return;

    if (tree->height == 0) {
        if (offset > 0) {
            for (int i = 0; i < offset - 2; i++)
                printf(" ");
            printf("->");
        }
        printf("[%2d", tree->keys[0]);

        if (tree->full == true)
            printf(", %2d", tree->keys[1]);
        printf("]\n");
    }
    else {
        PrintTree(tree->sons[0], offset + 6);
        if (tree->full == false) {
            if (offset > 0) {
                for (int i = 0; i < offset - 2; i++)
                    printf(" ");
                printf("->");
            }
            printf("%2d\n", tree->keys[0]);
            PrintTree(tree->sons[1], offset + 6);
        }
        else {
            for (int i = 0; i < offset - 2; i++)
                printf(" ");
            if (tree->parent != NULL)
                printf("->");
            printf("%2d\n", tree->keys[0]);
            PrintTree(tree->sons[1], offset + 6);
            for (int i = 0; i < offset; i++)
                printf(" ");
            printf("%2d\n", tree->keys[1]);
            PrintTree(tree->sons[2], offset + 6);
        }
    }
    if (offset == 0)
        fflush(stdout);
}


int main() {
    tree_t* tree = NULL;
    tree_t* temp = NULL;
    tree_t* found_node = NULL;

    int key;
    char func;
    while (scanf("%c", &func) >= 1) {
        if (func != 'q')
            scanf("%i", &key);
        switch (func) {
        case 'a': {
            temp = InsertNode(tree, key, NULL);
            if (temp == NULL) {
                DestroyTree(tree);
                return 0;
            }
            else
                tree = temp;
            break;
        }
        case 'r': {
            tree = DeleteNode(tree, key);
            break;
        }
        case 'f': {
            tree_t* found_node = FindNode(tree, key);
            if (found_node != NULL && (key == found_node->keys[0] || (found_node->full && key == found_node->keys[1])))
                printf("yes\n");
            else
                printf("no\n");
            break;
        }
        case 'p': {
            PrintTree(tree, 0);
            break;
        }
        case 'q':
            DestroyTree(tree);
            return 0;

        }
    }
    DestroyTree(tree);
    return 0;
}