#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#ifndef USE_AVL
#define USE_AVL 1
#endif

typedef struct Node {
    char *word;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

static Node* newNode(const char *word) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Out of memory\n");
        exit(FAILURE);
    }
    n->word = (char *)malloc(strlen(word) + 1);
    if (!n->word) {
        fprintf(stderr, "Out of memory\n");
        exit(EXIT_FAILURE);
    }
    strcpy(n->word, word);
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}
int treeHeight(Node *root) {
    if (root == NULL) return 0;
    int lh = treeHeight(root->left);
    int rh = treeHeight(root->right);
    return 1 + (lh > rh ? lh : rh);
}

int treeCount(Node *root) {
    if (root == NULL) return 0;
    return 1 + treeCount(root->left) + treeCount(root->right);
}

static int nodeHeight(Node *n) {
    return n ? n->height : 0;
}

static int balanceFactor(Node *n) {
    if (!n) return 0;
    return nodeHeight(n->left) - nodeHeight(n->right);
}

static void updateHeight(Node *n) {
    int lh = nodeHeight(n->left);
    int rh = nodeHeight(n->right);
    n->height = 1 + (lh > rh ? lh : rh);
}

static Node* rotateRight(Node *y) {
    Node *x  = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left  = T2;
    updateHeight(y);
    updateHeight(x);
    return x;
}

static Node* rotateLeft(Node *x) {
    Node *y  = x->right;
    Node *T2 = y->left;
    y->left  = x;
    x->right = T2;
    updateHeight(x);
    updateHeight(y);
    return y;
}

Node* insert(Node *root, const char *word) {
    if (root == NULL) return newNode(word);

    int cmp = strcasecmp(word, root->word);
    if (cmp < 0)
        root->left = insert(root->left, word);
    else if (cmp > 0)
        root->right = insert(root->right, word);
    else
        return root;






