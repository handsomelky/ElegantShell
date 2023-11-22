#ifndef BST_H__
#define BST_H__

typedef struct bst_node_st {
    char *cmd;
    int (*handler)(int argc, char **argv);
    struct bst_node_st *left, *right;
} BST_NODE;

BST_NODE *bst_insert(BST_NODE **root, const char *cmd, int (*handler)(int argc, char **argv));
BST_NODE *bst_search(BST_NODE *root, const char *cmd);

#endif
