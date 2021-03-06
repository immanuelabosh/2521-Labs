// Implementation of the BST ADT using an AVL tree

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Time.h"
#include "Tree.h"

typedef struct node *Node;
struct node {
    Time time;
    int  height;
    Node left;
    Node right;
};

struct tree {
    Node root;
};

////////////////////////////////////////////////////////////////////////
// Function Prototypes

static void doFree(Node n);
static Node doInsert(Node n, Time time);
static Node rotateLeft(Node n);
static Node rotateRight(Node n);
static int  height(Node n);
static int  max(int a, int b);
static Node newNode(Time time);
Time NodeTreeCeiling(Node n, Time time);


////////////////////////////////////////////////////////////////////////
// Constructor and Destructor

// Creates a new empty tree
Tree TreeNew(void) {
    Tree t = malloc(sizeof(*t));
    if (t == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }
    t->root = NULL;
    return t;
}

// Frees all memory associated with the given tree
void TreeFree(Tree t) {
    doFree(t->root);
    free(t);
}

static void doFree(Node n) {
    if (n != NULL) {
        doFree(n->left);
        doFree(n->right);
        TimeFree(n->time);
        free(n);
    }
}

////////////////////////////////////////////////////////////////////////
// Insertion

// Inserts  a  copy of the given time into the tree if it is not already
// in the tree
void TreeInsert(Tree t, Time time) {
    t->root = doInsert(t->root, time);
}

static Node doInsert(Node n, Time time) {
    if (n == NULL) {
        return newNode(time);
    }

    // insert recursively
    int cmp = TimeCmp(time, n->time);
    if (cmp < 0) {
        n->left = doInsert(n->left, time);
    } else if (cmp > 0) {
        n->right = doInsert(n->right, time);
    } else { // (cmp == 0)
        // if time is already in the tree,
        // we can return straight away
        return n;
    }

    // insertion done
    // correct the height of the current subtree
    n->height = 1 + max(height(n->left), height(n->right));
    
    // rebalance the tree
    // TODO: Add your code here and change
    //       the return statement if needed
 
    if ((height(n->left) - height(n->right)) > 1) {
        if (TimeCmp(time, n->left->time) > 0) {
            n->left = rotateLeft(n->left);
        }
        n = rotateRight(n);
    }
    if ((height(n->right) - height(n->left)) > 1) {
        if (TimeCmp(time, n->right->time) < 0) {
            n->right = rotateRight(n->right);
        }
       
        n = rotateLeft(n);
    }

    return n;
}

static Node newNode(Time time) {
    Node n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "Insufficient memory!\n");
        exit(EXIT_FAILURE);
    }

    n->time = TimeCopy(time);
    n->height = 0;
    n->left = NULL;
    n->right = NULL;
    return n;
}

// Rotates  the  given  subtree left and returns the root of the updated
// subtree.
static Node rotateLeft(Node n) {
		if (n == NULL) return n;
		if (n->right == NULL) return n;
		//make the node to the right the root
		Node root = n->right;
		//detach the old left child of the root and attach to the old root 
		n->right = root->left;
		//attach the old root to the left new root
		root->left = n;
		//get the max height of the children and update 
		n->height = 1 + max(height(n->right), height(n->left));
    root->height = 1 + max(height(root->right), height(root->left));
    return root;
}

// Rotates the given subtree right and returns the root of  the  updated
// subtree.
static Node rotateRight(Node n) {
		if (n == NULL) return n;
		if (n->left == NULL) return n;
		Node root = n->left;
		n->left = root->right;
		root->right = n;
		n->height = 1 + max(height(n->right), height(n->left));
    root->height = 1 + max(height(root->right), height(root->left));
    return root;
}

// Returns  the height of a subtree while assuming that the height field
// of the root node of the subtree is correct
static int height(Node n) {
    if (n == NULL) {
        return -1;
    } else {
        return n->height;
    }
}

static int max(int a, int b) {
    return a > b ? a : b;
}

////////////////////////////////////////////////////////////////////////
// Specialised Operations

// Returns the latest time in the tree that is earlier than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeFloor(Tree t, Time time) {
    // TODO: Add your code here and change
    //       the return statement if needed
    //       You can create helper functions
    //       if needed
		
		Node temp = t->root;
		Time floor = NULL;

		while (height(temp) >= 0){
			if (TimeCmp(temp->time, time) == 0) {
				floor = temp->time;
				break;
			}else if (TimeCmp(temp->time, time) < 0) {
				floor = temp->time;
				if (temp->right != NULL){
					temp = temp->right;
				}else {
					break;
				}
			}else {
				if (temp->left != NULL){
					temp = temp->left;
				}else {
					break;
				}
			}
		}

    return floor;
}


// Returns the earliest time in the tree that is later than or equal  to
// the  given  time,  or  NULL if no such time exists. The returned time
// should not be modified or freed.
Time TreeCeiling(Tree t, Time time) {
    // TODO: Add your code here and change
    //       the return statement if needed
    //       You can create helper functions
    //       if needed
	 Node temp = t->root;
	 return NodeTreeCeiling(temp, time);
}

Time NodeTreeCeiling(Node n, Time time){
		//if n is null
		if(n == NULL) return NULL;
		//n has the same time
		if (TimeCmp(n->time, time) == 0) return n->time;

		//if ceiling is after time 
		if ((TimeCmp(n->time, time) > 0) ){
			//check for earlier times that are valid ceiling 
			Time t = NodeTreeCeiling(n->left, time);
			//return n if earlier times are not valid ceilings
			return (t != NULL ? t : n->time);
		}

		//if ceiling is before time and there are later times
		if ((TimeCmp(n->time, time) < 0) ){
			//check for later times that are valid ceiling 
			return NodeTreeCeiling(n->right, time);
		}		
		//there are no later times
		return NULL;
}

////////////////////////////////////////////////////////////////////////
// Printing

//////////////////
// List the items

static void doList(Node n);

void TreeList(Tree t) {
    doList(t->root);
}

static void doList(Node n) {
    if (n != NULL) {
        doList(n->left);
        TimeShow(n->time);
        printf("\n");
        doList(n->right);
    }
}

///////////////////////////
// Show the tree structure

typedef unsigned long long uint64;
static void doShow(Node n, int level, uint64 arms);

void TreeShow(Tree t) {
    if (t->root != NULL && t->root->height >= 64) {
        printf("Tree is too tall!\n");
    } else {
        doShow(t->root, 0, 0);
    }
}

// This  function  uses a hack to determine when to draw the arms of the
// tree and relies on the tree being reasonably balanced. Don't  try  to
// use this function if the tree is not an AVL tree!
static void doShow(Node n, int level, uint64 arms) {
    if (n == NULL) return;

    TimeShow(n->time);
    printf(" (height: %d)\n", n->height);

    if (n->left != NULL) {
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("%s", n->right != NULL ? "┝━╸L: " : "┕━╸L: ");
        if (n->right != NULL) {
            arms |= (1LLU << level);
        } else {
            arms &= ~(1LLU << level);
        }
        doShow(n->left, level + 1, arms);
    }

    if (n->right != NULL) {
        // if (n->left != NULL) {
        //     for (int i = 0; i <= level; i++) {
        //         if ((1LLU << i) & arms) {
        //             printf("│     ");
        //         } else {
        //             printf("      ");
        //         }
        //     }
        //     printf("\n");
        // }
        for (int i = 0; i < level; i++) {
            if ((1LLU << i) & arms) {
                printf("│     ");
            } else {
                printf("      ");
            }
        }
        printf("┕━╸R: ");
        arms &= ~(1LLU << level);
        doShow(n->right, level + 1, arms);
    }
}

////////////////////////////////////////////////////////////////////////
// Testing
// All  functions below exist for testing purposes ONLY. Do NOT use them
// in your code.

static Node doInsertLeaf(Node n, Time time);

void TreeRotateLeftAtRoot(Tree t) {
    t->root = rotateLeft(t->root);
}

void TreeRotateRightAtRoot(Tree t) {
    t->root = rotateRight(t->root);
}

void TreeInsertLeaf(Tree t, Time time) {
    t->root = doInsertLeaf(t->root, time);
}

static Node doInsertLeaf(Node n, Time time) {
    if (n == NULL) {
        return newNode(time);
    }

    int cmp = TimeCmp(time, n->time);
    if (cmp < 0) {
        n->left = doInsertLeaf(n->left, time);
    } else if (cmp > 0) {
        n->right = doInsertLeaf(n->right, time);
    }

    n->height = 1 + max(height(n->left), height(n->right));
    return n;
}
