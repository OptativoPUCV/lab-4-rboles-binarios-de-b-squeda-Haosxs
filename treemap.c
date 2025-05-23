#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap*) malloc(sizeof(TreeMap));
    if (new == NULL) return NULL;
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (searchTreeMap(tree, key) != NULL) return;

    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    if (newNode == NULL) return;
    Pair* newPair = (Pair*)malloc(sizeof(Pair));
    if (newPair == NULL) {
        free(newNode);
        return;
    }

    newPair->key = key;
    newPair->value = value;
    newNode->pair = newPair;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    if (tree->root == NULL) {
        tree->root = newNode;
        tree->current = newNode;
        return;
    }
    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL) {
        parent = current;
        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;
    if (tree->lower_than(key, parent->pair->key))
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;

    while (x->left != NULL) {
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL; 
        } else {
            if (node->parent->left == node)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;

        if (node->parent == NULL) {
            tree->root = child;
        } else {
            if (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
        }
        child->parent = node->parent;
        return;
    }
    TreeNode* minRight = minimum(node->right);
    node->pair->key = minRight->pair->key;
    node->pair->value = minRight->pair->value;
    removeNode(tree, minRight);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;
    while (aux != NULL) {
        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        }
        else if (tree->lower_than(aux->pair->key, key)) {
            aux = aux->right;
        }
        else {
            tree->current = aux;
            return aux->pair;
        }
    }
    return NULL;
}

Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* node = tree->root;
    TreeNode* aux = NULL;

    while (node != NULL) {
        if (!tree->lower_than(node->pair->key, key) && !tree->lower_than(key, node->pair->key)) {
            tree->current = node;
            return node->pair;
        }
        if (tree->lower_than(key, node->pair->key)) {
            aux = node;
            node = node->left;
        } else {
            node = node->right;
        }
    }
    if (aux != NULL) {
        tree->current = aux;
        return aux->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* min = minimum(tree->root);
    tree->current = min;

    return (min != NULL) ? min->pair : NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode* node = tree->current;

    if (node->right != NULL) {
        TreeNode* minNode = minimum(node->right);
        tree->current = minNode;
        return minNode->pair;
    }

    TreeNode* parent = node->parent;
    while (parent != NULL && parent->right == node) {
        node = parent;
        parent = parent->parent;
    }

    tree->current = parent;
    return (parent != NULL) ? parent->pair : NULL;
}
