//JSR
#include "main.h"

struct _node
{
    int             data;
    struct _node*   left;
    struct _node*   right;
    int             balance;

    // balance will be right - left. +ve is right heavy; -ve is left heavy. Tree balancing only on mod(balance) >1
};
typedef struct _node node;

// The tree at pRoot is right heavy. Peform a single left rotation on it
// return the new parent
node* SingleLeftRotate(node* pRoot)
{
        node* rightSubTree=NULL, *leftChildOfRightSubTree=NULL;

    rightSubTree = pRoot->right;

    if(!rightSubTree) return pRoot;

    leftChildOfRightSubTree = rightSubTree->left;

    pRoot->right            = leftChildOfRightSubTree;

    rightSubTree->left      = pRoot;

    return rightSubTree;
}

// The tree rooted at pRoot is left heavy. Perform right rotation
// return the new parent
node* SingleRightRotate(node* pRoot)
{
        node* leftSubTree=NULL, *rightChildOfLeftSubTree=NULL;

    leftSubTree = pRoot->left;

    if(!leftSubTree) return pRoot;

    rightChildOfLeftSubTree = leftSubTree->right;

    pRoot->left    = rightChildOfLeftSubTree;

    leftSubTree->right  = pRoot;

    return leftSubTree;
}

// The tree rooted at pRoot is left heavy; but the left child is right heavy. Hence, perform left rotation on left child and then right rotation on root
node* DoubleRightRotate(node* pRoot)
{
        node* leftSubTree;

    leftSubTree = pRoot->left;
    if(!leftSubTree) return pRoot;

    // Perform left rotate on left sub tree
    pRoot->left = SingleLeftRotate(leftSubTree);

    return (SingleRightRotate(pRoot));
}

// The tree rooted at pRoot is right heavy; but the right subtree is left heavy. Perform right rotate on right child and then left rotate on root
node* DoubleLeftRotate(node* pRoot)
{
        node* rightSubTree;
    if(!(rightSubTree    = pRoot->right))return pRoot;

    // Perform right rotate on right sub tree
    pRoot->right    = SingleRightRotate(rightSubTree);

    return (SingleLeftRotate(pRoot));
}


node* NewNode(int pData)
{
        node* newNode;

    newNode             = (node*)malloc(sizeof(node));
    newNode->balance    = 0;
    newNode->left       = NULL;
    newNode->right      = NULL;
    newNode->data       = pData;

    return newNode;
}


node* BalanceTree(node* pRoot)
{
        int currBalance;

    if(pRoot==NULL) return NULL;

    currBalance = pRoot->balance;
    
    // Left heavy
    if(currBalance<-1)
    {
        if(currBalance==-2)
        {

        }
        else
        {
            

        }
    }
    // Right Heavy
    else if(currBalance>1)
    {

    }
    

}

node* InsertInTree(node* pRoot, int pData)
{
        int     currNodeData, childBalance;
        node*   newNode;

    if(!pRoot)
    {
        newNode = NewNode(pData);
        return newNode;
    }

    currNodeData = pRoot->data;

    if(pData <= currNodeData)
    {
        if(pRoot->left==NULL)
        {
            pRoot->left     = NewNode(pData); 
        }
        else
        {
            InsertInTree(pRoot->left,pData);    
        }
        pRoot->balance--;
    }
    else
    {
        if(pRoot->right == NULL)
        {
            pRoot->right = NewNode(pData);
        }
        else
        {
            InsertInTree(pRoot->right, pData);
        }
        pRoot->balance++;
    }
}

int AVLTreeDemo(int* pDataArray, int pIndex)
{
        int     n;
        node*   tree=NULL;

    for(n=0;n<pIndex;n++)
    {
        tree = InsertInTree(tree,pDataArray[n]);
    }
    return 0;

}