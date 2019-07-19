/***********************************************************************
* Component:
* Week 09, Binary Search Tree (BST)
* Brother Helfrich, CS 235
* Author:
* Br. Helfrich
* Summary:
* Create a binary search tree
************************************************************************/

#ifndef BST_H
#define BST_H

#ifndef debug
#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif // DEBUG
#endif // !debug

#include <cassert>

namespace custom
{

/*****************************************************************
* BINARY SEARCH TREE
* Create a Binary Search Tree
*****************************************************************/
template <class T>
class BST
{
public:
    // constructors, destructor, assignment operator
    BST() : root(NULL) {}
    BST(const BST &rhs);
    ~BST() { clear(); }
    BST &operator=(const BST &rhs);

    // standard container interfaces
    bool empty() const { return root == NULL; }
    int size() const { return empty() ? 0 : root->size(); }
    void clear()
    {
        if (root)
            deleteBinaryTree(root);
        assert(empty());
    }

    // iterators
    class iterator;
    iterator begin();
    iterator end() { return iterator(NULL); }
    iterator rbegin();
    iterator rend() { return iterator(NULL); }

    // BST specific interfaces
    void insert(const T &t);
    void remove(iterator &it);
    iterator find(const T &t);

private:
    // for debug purposes so we can validate the tree. This should be private
    class BNode;
    BNode *getRoot() { return root; }

    // delete a single node from the tree
    void deleteNode(BNode *&pDelete, bool toRight);
    void deleteBinaryTree(BNode *&pDelete);
    void copyBinaryTree(const BNode *pSrc,
                        BNode *pDest);

    BNode *root;
};

/*****************************************************************
* BINARY NODE
* A single node in a binary tree. Note that the node does not know
* anything about the properties of the tree so no validation can be done.
*****************************************************************/
template <class T>
class BST<T>::BNode
{
public:
    // constructor: set the pointers to NULL
    BNode() : pLeft(NULL), pRight(NULL), pParent(NULL),
              data(), isRed(true) {}
    BNode(const T &t) : pLeft(NULL), pRight(NULL), pParent(NULL),
                        data(t), isRed(true) {}

    // size
    int size() const
    {
        return 1 +
               (pLeft == NULL ? 0 : pLeft->size()) +
               (pRight == NULL ? 0 : pRight->size());
    }

    // add a node the left/right
    void addLeft(BNode *pNode);
    void addRight(BNode *pNode);

    // create a node and add it to the left/right
    void addLeft(const T &t);
    void addRight(const T &t);

    // is the passed node our right child?
    bool isRightChild(BNode *pNode) const { return pRight == pNode; }
    bool isLeftChild(BNode *pNode) const { return pLeft == pNode; }

    // balance the tree
    void balance();

#ifdef DEBUG
    // verify
    void verifyBTree() const;

    // how deep is the black nodes?
    int findDepth() const;

    // make sure all four red-black rules are followed
    void verifyRedBlack(int depth) const;
#endif // DEBUG

    // since no validation is done, everything is public
    BNode *pLeft;
    BNode *pRight;
    BNode *pParent;

    // the data of unknown type: cannot validate so is public
    T data;
    bool isRed;
};

/******************************************************
* BINARY NODE :: ADD LEFT
* Add a node to the left of the current node
******************************************************/
template <class T>
void BST<T>::BNode ::addLeft(BNode *pNode)
{
    pLeft = pNode;
    if (pNode)
        pNode->pParent = this;
}

/******************************************************
* BINARY NODE :: ADD RIGHT
* Add a node to the right of the current node
******************************************************/
template <class T>
void BST<T>::BNode ::addRight(BNode *pNode)
{
    pRight = pNode;
    if (pNode)
        pNode->pParent = this;
}

/******************************************************
* BINARY NODE :: ADD LEFT
* Add a node to the left of the current node
******************************************************/
template <class T>
void BST<T>::BNode ::addLeft(const T &t)
{
    assert(pLeft == NULL);
    assert(!(t > data));

    try
    {
        BNode *pNode = new BNode(t);
        addLeft(pNode);

        // paranoia and double-checks
        debug(verifyBTree());
        debug(std::cerr << '\t' << data << "->addLeft(" << t << ") ");

        pNode->balance();
    }
    catch (...)
    {
        throw "ERROR: Unable to allocate a node";
    }
}

/******************************************************
* BINARY NODE :: ADD RIGHT
* Add a node to the right of the current node
******************************************************/
template <class T>
void BST<T>::BNode ::addRight(const T &t)
{
    assert(pRight == NULL);
    assert(t > data || t == data);

    try
    {
        BNode *pNode = new BNode(t);
        addRight(pNode);

        // paranoia and double-checks
        debug(verifyBTree());
        debug(std::cerr << '\t' << data << "->addRight(" << t << ") ");

        pNode->balance();
    }
    catch (...)
    {
        throw "ERROR: Unable to allocate a node";
    }
}

/*****************************************************
* DELETE BINARY TREE
* Delete all the nodes below pThis including pThis
* using postfix traverse: LRV
****************************************************/
template <class T>
void BST<T>::deleteBinaryTree(BNode *&pDelete)
{
    if (pDelete == NULL)
        return;
    deleteBinaryTree(pDelete->pLeft);
    deleteBinaryTree(pDelete->pRight);
    delete pDelete;
    pDelete = NULL;
}

/**********************************************
* COPY BINARY TREE
* Copy pSrc->pRight to pDest->pRight and
* pSrc->pLeft onto pDest->pLeft
*********************************************/
template <class T>
void BST<T>::copyBinaryTree(const BNode *pSrc,
                            BNode *pDest)
{
    BNode *p = NULL;
    assert(pSrc && pDest);
    assert(pDest->pLeft == NULL && pDest->pRight == NULL);

    try
    {
        if (pSrc->pRight)
        {
            p = new BNode(pSrc->pRight->data);
            p->isRed = pSrc->pRight->isRed;
            pDest->addRight(p);
            copyBinaryTree(pSrc->pRight, pDest->pRight);
        }
        if (pSrc->pLeft)
        {
            p = new BNode(pSrc->pLeft->data);
            p->isRed = pSrc->pLeft->isRed;
            pDest->addLeft(p);
            copyBinaryTree(pSrc->pLeft, pDest->pLeft);
        }
    }
    catch (...)
    {
        throw "ERROR: Unable to allocate a node";
    }
}

#ifdef DEBUG
/****************************************************
* BINARY NODE :: FIND DEPTH
* Find the depth of the black nodes. This is useful for
* verifying that a given red-black tree is valid
****************************************************/
template <class T>
int BST<T>::BNode ::findDepth() const
{
    // if there are no children, the depth is ourselves
    if (pRight == NULL && pLeft == NULL)
        return (isRed ? 0 : 1);

    // if there is a right child, go that way
    if (pRight != NULL)
        return (isRed ? 0 : 1) + pRight->findDepth();
    else
        return (isRed ? 0 : 1) + pLeft->findDepth();
}

/****************************************************
* BINARY NODE :: VERIFY RED BLACK
* Do all four red-black rules work here?
***************************************************/
template <class T>
void BST<T>::BNode ::verifyRedBlack(int depth) const
{
    depth -= (isRed == false) ? 1 : 0;

    // Rule a) Every node is either red or black
    assert(isRed == true || isRed == false); // this feels silly

    // Rule b) The root is black
    if (pParent == NULL)
        assert(isRed == false);

    // Rule c) Red nodes have black children
    if (isRed == true)
    {
        if (pLeft != NULL)
            assert(pLeft->isRed == false);
        if (pRight != NULL)
            assert(pRight->isRed == false);
    }

    // Rule d) Every path from a leaf to the root has the same # of black nodes
    if (pLeft == NULL && pRight && NULL)
        assert(depth == 0);
    if (pLeft != NULL)
        pLeft->verifyRedBlack(depth);
    if (pRight != NULL)
        pRight->verifyRedBlack(depth);
}

/******************************************************
* VERIFY B TREE
* Verify that the tree is correctly formed
******************************************************/
template <class T>
void BST<T>::BNode ::verifyBTree() const
{
    // check parent
    if (pParent)
        assert(pParent->pLeft == this || pParent->pRight == this);

    // check left
    if (pLeft)
    {
        assert(pLeft->data <= data);
        assert(pLeft->pParent == this);
        pLeft->verifyBTree();
    }

    // check right
    if (pRight)
    {
        assert(pRight->data >= data);
        assert(pRight->pParent == this);
        pRight->verifyBTree();
    }
}
#endif // DEBUG

/******************************************************
* BINARY NODE :: BALANCE
* Balance the tree from a given location
******************************************************/
template <class T>
void BST<T>::BNode ::balance()
{
    debug(verifyBTree());

    // Case 1: if we are the root, then color ourselves black and call it a day.
    if (pParent == NULL)
    {
        isRed = false;
        debug(std::cerr << "Case 1\n");
        return;
    }

    // Case 2: if the parent is black, then there is nothing left to do
    if (pParent->isRed == false)
    {
        debug(std::cerr << "Case 2\n");
        return;
    }

    // we better have a grandparent. Otherwise there is a red node at the root
    assert(pParent->pParent != NULL);

    // find my relatives
    BNode *pGranny = pParent->pParent;
    BNode *pGreatG = pGranny->pParent;
    BNode *pSibling =
        pParent->isRightChild(this) ? pParent->pLeft : pParent->pRight;
    BNode *pAunt =
        pGranny->isRightChild(pParent) ? pGranny->pLeft : pGranny->pRight;

    // verify things are as they should be
    assert(pGranny != NULL);         // I should have a grandparent here
    assert(pGranny->isRed == false); // if granny is red, we violate red-red!

    // Case 3: if the aunt is red, then just recolor
    if (pAunt != NULL && pAunt->isRed == true)
    {
        // verification
        debug(std::cerr << "Case 3, ");
        debug(pGranny->verifyBTree());

        // perform Case 3
        pGranny->isRed = true;  // grandparent becomes red
        pParent->isRed = false; // parent becomes black
        pAunt->isRed = false;   // aunt becomes black
        pGranny->balance();     // balance granny!
        return;
    }

    // Case 4: if the aunt is black or non-existant, then we need to rotate
    assert(pParent->isRed == true && pGranny->isRed == false &&
           (pAunt == NULL || pAunt->isRed == false));
    debug(pGranny->verifyBTree());

    // the new top of the sub-tree
    BNode *pHead = NULL;

    // Case 4a: We are mom's left and mom is granny's left
    if (pParent->isLeftChild(this) && pGranny->isLeftChild(pParent))
    {
        // verify case 4a is as it should be
        debug(std::cerr << "Case 4a\n");
        debug(pGranny->verifyBTree());
        assert(pParent->pLeft == this);
        assert(pGranny->pRight == pAunt);
        assert(pGranny->isRed == false);

        // perform the necessary rotation
        pParent->addRight(pGranny);
        pGranny->addLeft(pSibling);
        pHead = pParent;

        // set the colors
        pParent->isRed = false;
        pGranny->isRed = true;

        // final checks that we do not violate the red-black nature
    }

    // case 4b: We are mom's right and mom is granny's right
    else if (pParent->isRightChild(this) && pGranny->isRightChild(pParent))
    {
        // verify case 4b is as it should be
        debug(std::cerr << "Case 4b\n");
        debug(pGranny->verifyBTree());
        assert(pParent->pRight == this);
        assert(pGranny->pLeft == pAunt);
        assert(pGranny->isRed == false);

        // perform the necessary rotation
        pParent->addLeft(pGranny);
        pGranny->addRight(pSibling);
        pHead = pParent;

        // set the colors
        pParent->isRed = false;
        pGranny->isRed = true;
    }

    // Case 4c: We are mom's right and mom is granny's left
    else if (pParent->isRightChild(this) && pGranny->isLeftChild(pParent))
    {
        // verify case 4c is as it should be
        debug(std::cerr << "Case 4c\n");
        assert(pGranny->pRight == pAunt);
        assert(pParent->pLeft == pSibling);
        assert(pParent->isRed == true);

        // perform the necessary rotation
        pGranny->addLeft(this->pRight);
        pParent->addRight(this->pLeft);
        this->addRight(pGranny);
        this->addLeft(pParent);
        pHead = this;

        // set the colors
        this->isRed = false;
        pGranny->isRed = true;
    }

    // case 4d: we are mom's left and mom is granny's right
    else if (pParent->isLeftChild(this) && pGranny->isRightChild(pParent))
    {
        // verify case 4d is as it should be
        debug(std::cerr << "Case 4d\n");
        assert(pGranny->pLeft == pAunt);
        assert(pGranny->pRight == pParent);
        assert(pParent->pRight == pSibling);

        // perform the necessary rotation
        pGranny->addRight(this->pLeft);
        pParent->addLeft(this->pRight);
        this->addLeft(pGranny);
        this->addRight(pParent);
        pHead = this;

        // set the colors
        this->isRed = false;
        pGranny->isRed = true;
    }

    // else we are really confused!
    else
    {
        assert(false); // !!
    }

    // fix up great granny if she is not null
    if (pGreatG == NULL)
        pHead->pParent = NULL;
    else if (pGreatG->pRight == pGranny)
        pGreatG->addRight(pHead);
    else if (pGreatG->pLeft == pGranny)
        pGreatG->addLeft(pHead);

    debug(pHead->verifyBTree());
}

/**********************************************************
* BINARY SEARCH TREE ITERATOR
* Forward and reverse iterator through a BST
*********************************************************/
template <class T>
class BST<T>::iterator
{
public:
    // constructors and assignment
    iterator(BNode *p = NULL) { pNode = p; }
    iterator(const iterator &rhs) { pNode = rhs.pNode; }
    iterator &operator=(const iterator &rhs)
    {
        pNode = rhs.pNode;
        return *this;
    }

    // compare
    bool operator==(const iterator &rhs) const
    {
        // only need to compare the leaf node
        return rhs.pNode == pNode;
    }
    bool operator!=(const iterator &rhs) const
    {
        // only need to compare the leaf node
        return rhs.pNode != pNode;
    }

    // de-reference. Cannot change because it will invalidate the BST
    T &operator*()
    {
        return pNode->data;
    }

    // iterators
    iterator &operator++();
    iterator operator++(int postfix)
    {
        iterator itReturn = *this;
        ++(*this);
        return itReturn;
    }
    iterator &operator--();
    iterator operator--(int postfix)
    {
        iterator itReturn = *this;
        --(*this);
        return itReturn;
    }

    // must give friend status to remove so it can call getNode() from it
    friend void BST<T>::remove(iterator &it);

private:
    // get the node pointer
    BNode *getNode() { return pNode; }

    // the stack of nodes
    BNode *pNode;
};

/*****************************************************
* BST :: BEGIN
* Return the first node (left-most) in a binary search tree
****************************************************/
template <class T>
typename BST<T>::iterator custom ::BST<T>::begin()
{
    // if the BST is empty, return the NULL iterator.
    if (root == NULL)
        return end();

    // otherwise, find the left-most node;
    BNode *p = root;
    while (p->pLeft)
        p = p->pLeft;
    return iterator(p);
}

/*****************************************************
* BST :: RBEGIN
* Return the last node (right-most) in a binary search tree
****************************************************/
template <class T>
typename BST<T>::iterator BST<T>::rbegin()
{
    // if the BST is empty, return the NULL iterator.
    if (root == NULL)
        return end();

    // otherwise, find the left-most node;
    BNode *p = root;
    while (p->pRight)
        p = p->pRight;
    return iterator(p);
}

/*********************************************
* BST :: COPY CONSTRUCTOR
* Copy one tree to another
********************************************/
template <class T>
BST<T>::BST(const BST<T> &rhs) : root(NULL)
{
    // call the assignment operator
    *this = rhs;
}

/**********************************************
* BST :: COPY
* Make a copy of a binary search tree
**********************************************/
template <class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs)
{
    clear();
    if (rhs.root != NULL)
    {
        try
        {
            this->root = new BNode(rhs.root->data);
            copyBinaryTree(rhs.root, this->root);
        }
        catch (...)
        {
            throw "ERROR: Unable to allocate a node";
        }
    }
    return *this;
}

/*****************************************************
* BST :: INSERT
* Insert a node at a given location in the tree
****************************************************/
template <class T>
void BST<T>::insert(const T &t)
{
    try
    {
        // if we are at a trivial state (empty tree), then create a new root
        if (root == NULL)
        {
            root = new BNode(t);
            root->isRed = false;
            return;
        }

        // otherwise, go a searching for the correct spot
        BNode *node = root;
        bool done = false;
        while (!done)
        {
            // if the center node is larger, go left
            if (node->data > t)
            {
                // if there is a node to the left, follow it
                if (node->pLeft)
                    node = node->pLeft;
                // if we are at the leaf, then create a new node
                else
                {
                    node->addLeft(t);
                    done = true;
                }
            }

            // if the center node is smaller, go right
            else
            {
                // if there is a node to the right, follow it
                if (node->pRight)
                    node = node->pRight;
                // if we are at the left, then create a new node.
                else
                {
                    node->addRight(t);
                    done = true;
                }
            }
        }
        assert(root != NULL); // we just inserted something!

        // if the root moved out from under us, find it again.
        while (root->pParent != NULL)
            root = root->pParent;
        assert(root->pParent == NULL);
    }
    catch (...)
    {
        throw "ERROR: Unable to allocate a node";
    }
}

/*************************************************
* BST :: REMOVE
* Remove a given node as specified by the iterator
************************************************/
template <class T>
void BST<T>::remove(iterator &it)
{
    // find the node
    BNode *pNode = it.getNode();

    // do nothing if there is nothing to do
    if (pNode == NULL)
        return;

    // if there is only one child (right) or no children (how sad!)
    if (pNode->pLeft == NULL)
        deleteNode(pNode, true /* goRight */);

    // if there is only one child (left)
    else if (pNode->pRight == NULL)
        deleteNode(pNode, false /* goRight */);

    // otherwise, swap places with the in-order successor
    else
    {
        // find the in-order successor
        BNode *pNodeIOS = pNode->pRight;
        while (pNodeIOS->pLeft != NULL)
            pNodeIOS = pNodeIOS->pLeft;

        // copy its data
        pNode->data = pNodeIOS->data;

        // if there are any children under the in-order successor, fix them
        assert(pNodeIOS->pLeft == NULL); // there cannot be a left child or
        // I would not be the IOS
        deleteNode(pNodeIOS, true /*goRight*/);

        // prepare for deletion
        pNode = pNodeIOS;
    }

    delete pNode;
}

/****************************************************
* DELETE NODE
* Delete a single node (pDelete) from the tree indicated
* by a node (pDelete). 
* pDelete the node to be deleted
* toRight should the right branch inherit our place?
****************************************************/
template <class T>
void BST<T>::deleteNode(BNode *&pDelete, bool toRight)
{
    // shift everything up
    BNode *pNext = (toRight ? pDelete->pRight : pDelete->pLeft);

    // if we are not the parent, hook ourselves into the existing tree
    if (pDelete != root)
    {
        if (pDelete->pParent->pLeft == pDelete)
        {
            pDelete->pParent->pLeft = NULL;
            pDelete->pParent->addLeft(pNext);
        }
        else
        {
            pDelete->pParent->pRight = NULL;
            pDelete->pParent->addRight(pNext);
        }
    }

    // otherwise, the pNext is the new root
    else
    {
        root = pNext;
        pNext->pParent = NULL;
    }
}

/****************************************************
* BST :: FIND
* Return the node corresponding to a given value
****************************************************/
template <class T>
typename BST<T>::iterator BST<T>::find(const T &t)
{
    // perform a binary search using a non-recursive solution
    BNode *p = root;
    while (p)
    {
        if (p->data == t)
            return iterator(p);
        if (p->data > t)
            p = p->pLeft;
        else
            p = p->pRight;
    }

    // nothing was found so return the NULL iterator
    return end();
}

/**************************************************
* BST ITERATOR :: INCREMENT PREFIX
* advance by one
*************************************************/
template <class T>
typename BST<T>::iterator &BST<T>::iterator ::operator++()
{
    // do nothing if we have nothing
    if (NULL == pNode)
        return *this;

    // if there is a right node, take it
    if (NULL != pNode->pRight)
    {
        // go right
        pNode = pNode->pRight;

        // jig left - there might be more left-most children
        while (pNode->pLeft)
            pNode = pNode->pLeft;
        return *this;
    }

    // there are no right children, the left are done
    assert(NULL == pNode->pRight);
    BNode *pSave = pNode;

    // go up...
    pNode = pNode->pParent;

    // if the parent is the NULL, we are done!
    if (NULL == pNode)
        return *this;

    // if we are the left-child, got to the parent.
    if (pSave == pNode->pLeft)
        return *this;

    // we are the right-child, go up as long as we are the right child!
    while (NULL != pNode && pSave == pNode->pRight)
    {
        pSave = pNode;
        pNode = pNode->pParent;
    }

    return *this;
}

/**************************************************
* BST ITERATOR :: DECREMENT PREFIX
* advance by one
*************************************************/
template <class T>
typename BST<T>::iterator &BST<T>::iterator ::operator--()
{
    // do nothing if we have nothing
    if (NULL == pNode)
        return *this;

    // if there is a left node, take it
    if (NULL != pNode->pLeft)
    {
        // go left
        pNode = pNode->pLeft;

        // jig right - there might be more right-most children
        while (pNode->pRight)
            pNode = pNode->pRight;
        return *this;
    }

    // there are no left children, the right are done
    assert(NULL == pNode->pLeft);
    BNode *pSave = pNode;

    // go up
    pNode = pNode->pParent;

    // if the parent is the NULL, we are done!
    if (NULL == pNode)
        return *this;

    // if we are the right-child, got to the parent.
    if (pSave == pNode->pRight)
        return *this;

    // we are the left-child, go up as long as we are the left child!
    while (NULL != pNode && pSave == pNode->pLeft)
    {
        pSave = pNode;
        pNode = pNode->pParent;
    }

    return *this;
}

} // namespace custom

#endif // BST_H