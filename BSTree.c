#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"


/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
static struct treeNode* createNode(int data)
{
    struct treeNode* newNode = (struct treeNode*)malloc(sizeof(struct treeNode));
   
    if(newNode == NULL)
        return NULL;
   
    else
    {
        newNode->data = data;
        newNode->left = NULL;
        newNode->right = NULL;
    }
	return newNode;
}

/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
    int nodes = numberOfNodes(tree);
    int* arr = (int*)calloc(nodes, sizeof(int));
    
    fillArray(tree, arr, 0);
    
    return arr;
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    *tree = arrayToTree(arr, 0, size-1);
}

/* Implementation av tradet, funktionerna i interfacet */

BSTree emptyTree(void)
{
	return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
	if(tree == NULL)
        return 1;
    else
        return 0;
}

/* Satter in 'data' sorterat i *tree*/

void insertSorted(BSTree* tree, int data)
{
    if(find(*tree, data) == 1)
    {
        printf("Duplicates not allowed\n");
        return;
    }
    
    if(*tree == NULL)
    {
        struct treeNode* newNode = createNode(data);
        if(newNode == NULL)
            return;
        *tree = newNode;
        assert(find(*tree, data) == 1);
    }
    else
    {
       if(data > (*tree)->data)
           insertSorted(&(*tree)->right, data);
       else
           insertSorted(&(*tree)->left, data);
    }
    
}

/* Utskriftsfunktioner */

void printPreorder(const BSTree tree, FILE *textfile)
{
    if(tree != NULL)
    {
        fprintf(textfile, "%d ", tree->data);
        printPreorder(tree->left, stdout);
        printPreorder(tree->right, stdout);
    }
}

void printInorder(const BSTree tree, FILE *textfile)
{
    if(tree != NULL)
    {
        printInorder(tree->left, stdout);
        fprintf(textfile, "%d ", tree->data);
        printInorder(tree->right, stdout);
    }
}

void printPostorder(const BSTree tree, FILE *textfile)
{
    if(tree != NULL)
    {
        printPostorder(tree->left, stdout);
        printPostorder(tree->right, stdout);
        fprintf(textfile, "%d ", tree->data);
    }
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
    if(tree == NULL)
        return 0;
	
    if(tree->data == data)
        return 1;
    
    if(data > tree->data)
        return find(tree->right, data);
    
    else
        return find(tree->left, data);
    
    
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree* tree, int data)
{
	
    if(*tree == NULL)
        return;
    
    if(data > (*tree)->data)
        removeElement(&(*tree)->right, data);
    
    
    else if(data < (*tree)->data)
        removeElement(&(*tree)->left, data);
    
    else
    {
        if((*tree)->right == NULL && (*tree)->left == NULL) // L…V
        {
            free(*tree);
            *tree = NULL;
        }
        
        else if((*tree)->right == NULL) //ETT BARN
        {
            BSTree temp = (*tree)->left;
            free(*tree);
            *tree = temp;
        }
        else if((*tree)->left == NULL) //ETT BARN
        {
            BSTree temp = (*tree)->right;
            free(*tree);
            *tree = temp;
        }
        
        else // TV BARN
        {
            BSTree temp = (*tree)->left;
            while(temp->right != NULL)
                temp = temp->right;
            (*tree)->data = temp->data;
            removeElement(&(*tree)->left, temp->data);
        }
    }
    
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
    int count = 1;
    
    if(tree == NULL)
        return 0;
    else
    {
        count = count + numberOfNodes(tree->right);
        count = count + numberOfNodes(tree->left);
        return count;
    }
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
	if(tree == NULL)
        return 0;
    else
    {
        int right = depth(tree->right);
        int left = depth(tree->left);
        
        if(right > left)
            return right+1;
        else
            return left+1;
    }
}

/* Returnerar minimidjupet for tradet*/
int minDepth(const BSTree tree)
{
    int n = numberOfNodes(tree);
    int minimumDepth = ceil(log2(n+1));
    
    return minimumDepth;
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree)
{
    if(*tree == NULL)
        return;
    
    int nodes = numberOfNodes(*tree);
    
    int *arr = writeSortedToArray(*tree);
    freeTree(tree);
    buildTreeSortedFromArray(tree, arr, nodes);
    free(arr);
    
    assert(minDepth(*tree) == depth(*tree));
    assert(numberOfNodes(*tree) == nodes);
}

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree)
{
	if(*tree == NULL)
        return;
    
    freeTree(&(*tree)->left);
    freeTree(&(*tree)->right);
    free(*tree);
    *tree = NULL;
}

void fillArray(BSTree tree, int array[], int i)
{
    if (tree == NULL)
        return;
    fillArray(tree->left, array, i+1);
    array[i] = tree->data;
    fillArray(tree->right, array, i+1);
}

struct treeNode* arrayToTree(const int arr[], int first, int last)
{
    if(first > last)
      return NULL;
    
    int middle = (first + last) / 2;
    
    BSTree root = createNode(arr[middle]);
    if(root == NULL)
    {
        printf("Memory alloation failed\n");
        return NULL;
    }
    else
    {
    root->left = arrayToTree(arr, first, middle-1);
    root ->right = arrayToTree(arr, middle+1, last);
    }
    return root;
}
