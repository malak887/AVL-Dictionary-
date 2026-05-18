#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>  
#include <ctype.h>
#ifndef USE_AVL
#define USE_AVL 1
#endif
#define MAX_WORD 100
#define MAX_SENTENCE 1000
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
        exit(EXIT_FAILURE);
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

int treeheight(Node *root) {
    if (root == NULL) return 0;
    int lh = treeheight(root->left);
    int rh = treeheight(root->right);
    return 1 + (lh > rh ? lh : rh);
}

int treecount(Node *root) {
    if (root == NULL) return 0;
    return 1 + treecount(root->left) + treecount(root->right);
}

#if USE_AVL
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
#endif

Node* insert(Node *root, const char *word) {
    if (root == NULL) return newNode(word);

    int cmp = strcasecmp(word, root->word);
    if (cmp < 0)
        root->left = insert(root->left, word);
    else if (cmp > 0)
        root->right = insert(root->right, word);
    else
        return root;               

#if USE_AVL
    updateHeight(root);
    int bf = balanceFactor(root);

    if (bf > 1 && strcasecmp(word, root->left->word) < 0)
        return rotateRight(root);
    
    if (bf < -1 && strcasecmp(word, root->right->word) > 0)
        return rotateLeft(root);
    if (bf > 1 && strcasecmp(word, root->left->word) > 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }
    if (bf < -1 && strcasecmp(word, root->right->word) < 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
#endif
    return root;
}
Node* search(Node *root, const char *word, Node **lastVisited) {
    Node *cur  = root;
    Node *last = NULL;
    while (cur != NULL) {
        last = cur;
        int cmp = strcasecmp(word, cur->word);
        if (cmp == 0) {
            if (lastVisited) *lastVisited = last;
            return cur;
        }
        cur = (cmp < 0) ? cur->left : cur->right;
    }
    if (lastVisited) *lastVisited = last;
    return NULL;
}

Node* inorderPredecessor(Node *root, Node *target) {
    if (root == NULL || target == NULL) return NULL;

    if (target->left != NULL) {
        Node *cur = target->left;
        while (cur->right != NULL) cur = cur->right;
        return cur;
    }

    Node *pred = NULL;
    Node *cur  = root;
    while (cur != NULL && cur != target) {
        int cmp = strcasecmp(target->word, cur->word);
        if (cmp > 0) {
            pred = cur;
            cur  = cur->right;
        } else {
            cur  = cur->left;
        }
    }
    return pred;
}

Node* inorderSuccessor(Node *root, Node *target) {
    if (root == NULL || target == NULL) return NULL;

    if (target->right != NULL) {
        Node *cur = target->right;
        while (cur->left != NULL) cur = cur->left;
        return cur;
    }

    Node *succ = NULL;
    Node *cur  = root;
    while (cur != NULL && cur != target) {
        int cmp = strcasecmp(target->word, cur->word);
        if (cmp < 0) {
            succ = cur;
            cur  = cur->left;
        } else {
            cur  = cur->right;
        }
    }
    return succ;
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root->word);
    free(root);
}

// loading the dictionary 
Node* loadDictionary(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Cannot open file!\n");
        return NULL;
    }
    Node *root = NULL;
    char word[MAX_WORD];

    while (fgets(word,sizeof(word),file)) {
        word[strcspn(word, "\n")] = '\0';
        root = insert(root, word);
    }
    fclose(file); return root;
}
//convert the input to lowercase , to facilitate the search
void toLowerCase(char *str) {

    for (int i = 0; str[i]; i++) {
        str[i]= tolower(str[i]);
    }
}

int main(){
     Node *root =loadDictionary("dictionary.txt");
    if (root == NULL)
        return 1;
    char sentence[MAX_SENTENCE];

    printf("\nEnter a sentence:\n");
    fgets(sentence,sizeof(sentence),stdin);
    char *token =strtok(sentence, " ,.!?;:\n\t");
    while (token != NULL) {
        toLowerCase(token);
        Node *lastVisited = NULL;
        Node *found =search(root,token,&lastVisited);
        if (found != NULL) {
            printf("\n%s is CORRECT\n",
                   token);
        }
        else {

            printf("\n%s is INCORRECT\n",token);
            printf("Suggestions:\n");
            if (lastVisited)
                printf("1. %s\n", lastVisited->word);

            Node *pred = inorderPredecessor(root, lastVisited);
            Node *succ = inorderSuccessor(root, lastVisited);

            printf("2. %s\n", pred ? pred->word : "None");
            printf("3. %s\n", succ ? succ->word : "None");
        }
        token =strtok(NULL," ,.!?;:\n\t");
    }

}