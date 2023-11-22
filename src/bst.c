#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

BST_NODE *bst_insert(BST_NODE **root, const char *cmd, int (*handler)(int argc, char **argv)) {
    if (*root == NULL) {
        *root = (BST_NODE *)malloc(sizeof(BST_NODE));
        if (*root == NULL) {
            return NULL; // Memory allocation failed
        }
        (*root)->cmd = strdup(cmd);
        (*root)->handler = handler;
        (*root)->left = (*root)->right = NULL;
        return *root;
    }

    int cmp = strcmp(cmd, (*root)->cmd);
    if (cmp < 0) {
        return bst_insert(&((*root)->left), cmd, handler);
    } else if (cmp > 0) {
        return bst_insert(&((*root)->right), cmd, handler);
    } else {
        // Command already exists
        return NULL;
    }
}

BST_NODE *bst_search(BST_NODE *root, const char *cmd) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(cmd, root->cmd);
    if (cmp < 0) {
        return bst_search(root->left, cmd);
    } else if (cmp > 0) {
        return bst_search(root->right, cmd);
    } else {
        return root;
    }
}
