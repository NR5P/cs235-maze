#ifndef LIST_H
#define LIST_H

namespace custom
{

/**************************************************
* LIST
* Generic linked list class. Supports the following:
* = : copy one list onto another
* ERASE : empty the list
* EMPTY : is the list currently empty?
* PUSH_BACK : add an item to the tail
* PUSH_FRONT : add an item to the head of the list
* BACK : returns the last element in the list
* FRONT : returns the first element in the list
* INSERT : add an item to a location somewhere in the middle
* iterators : forward and backward iterators, const and non-const
**************************************************/
template <typename T>
class list
{
public:
    // constructors, destructores, and assignment operator
    list() : numElements(0), pHead(NULL), pTail(NULL) {}
    list(const list<T> &rhs) : pHead(NULL), pTail(NULL), numElements(0)
    {
        Node *iter = rhs.pHead;

        //loop to copy over data, same as assignment =
        while (iter != nullptr)
        {
            this->push_back(iter->data);
            iter = iter->pNext;
        }
        //set the size
        numElements = rhs.numElements;
    }
    ~list() { clear(); }
    list<T> &operator=(const list<T> &rhs)
    {
        clear();
        Node *iter = rhs.pHead;
        while (iter != nullptr)
        {
            this->push_back(iter->data);
            iter = iter->pNext;
        }
        numElements = rhs.numElements;
        return *this;
    }

    // standard container interfaces
    void clear();
    bool empty() const { return size() == 0; }
    int size() const { return numElements; }

    // iterators
    class iterator;
    class const_iterator;
    class reverse_iterator;
    class const_reverse_iterator;
    iterator begin() { return iterator(pHead); }
    reverse_iterator rbegin() { return reverse_iterator(pTail); }
    const_iterator cbegin() const { return const_iterator(pHead); }
    iterator end() { return iterator(NULL); }
    reverse_iterator rend() { return reverse_iterator(NULL); }
    const_iterator cend() const { return const_iterator(NULL); }
    const_reverse_iterator crbegin() const
    {
        return const_reverse_iterator(pTail);
    }
    const_reverse_iterator crend() const
    {
        return const_reverse_iterator(NULL);
    }

    // list-specific interfaces
    void push_front(const T &data);
    void push_back(const T &data);
    T &front();
    T &back();
    iterator insert(iterator &it, const T &data);
    iterator erase(iterator it);
    iterator find(const T &t);

private:
    // nested linked list class
    class Node;
    Node *createNode();
    Node *createNode(const T &t);
    // member variables
    int numElements; // though we could count, it is faster to keep a variable
    Node *pHead;     // pointer to the beginning of the list
    Node *pTail;     // pointer to the ending of the list
};

/*************************************************
* LIST ITERATOR
* Iterate through a List backwards, non-constant version
************************************************/
template <typename T>
class list<T>::iterator
{
public:
    // constructors, destructors, and assignment operator
    iterator() : p(NULL) {}
    iterator(Node *p) : p(p) {}
    iterator(const iterator &rhs) { *this = rhs; }
    iterator &operator=(const iterator &rhs)
    {
        p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator==(const iterator &rhs) const
    {
        return rhs.p == this->p;
    }
    bool operator!=(const iterator &rhs) const
    {
        return rhs.p != this->p;
    }

    // dereference operator, fetch a node
    T &operator*()
    {
        if (p)
            return p->data;
        else
            throw "ERROR: Trying to dereference a NULL pointer";
    }

    // postfix increment
    iterator operator++(int postfix)
    {
        iterator old(*this);
        if (p)
            p = p->pNext;
        return old;
    }

    // prefix increment
    iterator &operator++()
    {
        if (p)
            p = p->pNext;
        return *this;
    }

    // postfix decrement
    iterator operator--(int postfix)
    {
        iterator old(*this);
        if (p)
            p = p->pPrev;
        return old;
    }

    // prefix decrement
    iterator &operator--()
    {
        if (p)
            p = p->pPrev;
        return *this;
    }

private:
    list<T>::Node *p;
    friend class list;
};

template <typename T>
typename list<T>::Node *list<T>::createNode()
{
    Node *p = new (std::nothrow) Node();
    if (p == nullptr)
        throw "ERROR: unable to access data from an empty list";
    return p;
}

template <typename T>
typename list<T>::Node *list<T>::createNode(const T &t)
{
    Node *p = createNode();
    p->data = t;
    return p;
}

template <typename T>
void list<T>::clear()
{
    list<T>::Node *pNode = pHead;
    while (pNode != nullptr)
    {
        list<T>::Node *pNext = pNode->pNext;
        delete pNode;
        pNode = pNext;
    }
    pHead = nullptr;
    pTail = nullptr;
    numElements = 0;
}

template <typename T>
void list<T>::push_front(const T &data)
{
    if (pHead == nullptr)
    {
        pHead = createNode(data);
        pTail = pHead;
    }
    else
    {
        Node *pNewNode = createNode(data);
        pNewNode->pNext = pHead;
        pHead->pPrev = pNewNode;
        pHead = pNewNode;
    }
    numElements++;
}

template <typename T>
void list<T>::push_back(const T &data)
{
    if (pTail == nullptr)
    {
        pTail = createNode(data);
        pHead = pTail;
    }
    else
    {
        Node *pNewNode = createNode(data);
        pNewNode->pPrev = pTail;
        pTail->pNext = pNewNode;
        pTail = pNewNode;
    }
    numElements++;
}

template <typename T>
T &list<T>::front()
{
    if (empty())
        throw "ERROR: unable to access data from an empty list";
    return pHead->data;
}

template <typename T>
T &list<T>::back()
{
    if (empty())
        throw "ERROR: unable to access data from an empty list";
    return pTail->data;
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator &it, const T &data)
{

    Node *pNew = createNode(data);

    if (empty())
    {
        pHead = pNew;
        pTail = pNew;
        return iterator(pNew);
    }

    if (it.p == nullptr && empty())
    {
        throw "No position specified";
    }

    if (it == end())
    {
        pTail->pNext = pNew;
        pNew->pPrev = pTail;
        pTail = pNew;
        it = pNew;
        numElements++;
        return iterator(pNew);
    }

    Node *pCurrent = it.p;
    Node *prev = pCurrent->pPrev;
    Node *next = pCurrent->pNext;

    if (prev == nullptr)
    {
        pHead = pNew;
        pCurrent->pPrev = pNew;
        pNew->pNext = pCurrent;
        return iterator(pNew);
    }

    if (next == nullptr)
    {
        pTail = pNew;
        pCurrent->pNext = pNew;
        pNew->pPrev = pCurrent;
        return iterator(pNew);
    }

    pNew->pPrev = prev;
    pNew->pNext = pCurrent;
    pCurrent->pPrev = pNew;
    prev->pNext = pNew;
    next->pPrev = pNew;
    numElements++;
    return iterator(pNew);
}

template <typename T>
typename list<T>::iterator list<T>::erase(iterator it)
{
    Node *prev = it.p->pPrev;
    Node *next = it.p->pNext;

    if (prev == nullptr)
    {
        //we are deleting the head
        delete it.p;
        next->pPrev = nullptr;
        pHead = next;
        return iterator(pHead);
    }

    if (next == nullptr)
    {
        delete it.p;
        prev->pNext = nullptr;
        pTail = prev;
        return iterator(prev);
    }

    prev->pNext = next;
    next->pPrev = prev;
    delete it.p;
    numElements--;
    return iterator(next);
}

template <typename T>
typename list<T>::iterator list<T>::find(const T &t)
{
    for (Node *p = pHead; p != nullptr; p = p->pNext)
    {
        if (t == p->data)
        {
            return iterator(p);
        }
    }

    return iterator(nullptr);
}

template <typename T>
class list<T>::Node
{
public:
    T data;
    Node *pNext;
    Node *pPrev;
    Node()
        : pNext(nullptr),
          pPrev(nullptr)
    {
    }
    Node(const T &t)
        : pNext(nullptr),
          pPrev(nullptr)
    {
        data = t;
    }
};

/*************************************************
* LIST REVERSE ITERATOR
* Iterate through a List backwards, non-constant version
************************************************/
template <typename T>
class list<T>::reverse_iterator
{
public:
    // constructors, destructors, and assignment operator
    reverse_iterator() : p(NULL) {}
    reverse_iterator(Node *p) : p(p) {}
    reverse_iterator(const reverse_iterator &rhs) { *this = rhs; }
    reverse_iterator &operator=(const reverse_iterator &rhs)
    {
        p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator==(const reverse_iterator &rhs) const
    {
        return rhs.p == this->p;
    }
    bool operator!=(const reverse_iterator &rhs) const
    {
        return rhs.p != this->p;
    }

    // dereference operator, fetch a node
    T &operator*()
    {
        if (p)
            return p->data;
        else
            throw "ERROR: Trying to dereference a NULL pointer";
    }

    // postfix increment
    reverse_iterator operator++(int postfix)
    {
        reverse_iterator old(*this);
        if (p)
            p = p->pPrev;
        return old;
    }

    // prefix increment
    reverse_iterator &operator++()
    {
        if (p)
            p = p->pPrev;
        return *this;
    }

    // postfix decrement
    reverse_iterator operator--(int postfix)
    {
        reverse_iterator old(*this);
        if (p)
            p = p->pNext;
        return old;
    }

    // prefix decrement
    reverse_iterator &operator--()
    {
        if (p)
            p = p->pNext;
        return *this;
    }

private:
    list<T>::Node *p;

    friend class list;
};

/*************************************************
* LIST ITERATOR
* Iterate through a List backwards, non-constant version
************************************************/
template <typename T>
class list<T>::const_iterator
{
public:
    // constructors, destructors, and assignment operator
    const_iterator() : p(NULL) {}
    const_iterator(Node *p) : p(p) {}
    const_iterator(const const_iterator &rhs) { *this = rhs; }
    const_iterator &operator=(const const_iterator &rhs)
    {
        p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator==(const const_iterator &rhs) const
    {
        return rhs.p == this->p;
    }
    bool operator!=(const const_iterator &rhs) const
    {
        return rhs.p != this->p;
    }

    // dereference operator, fetch a node
    T &operator*()
    {
        if (p)
            return p->data;
        else
            throw "ERROR: Trying to dereference a NULL pointer";
    }

    // postfix increment
    const_iterator operator++(int postfix)
    {
        const_iterator old(*this);
        if (p)
            p = p->pNext;
        return old;
    }

    // prefix increment
    const_iterator &operator++()
    {
        if (p)
            p = p->pNext;
        return *this;
    }

    // postfix decrement
    const_iterator operator--(int postfix)
    {
        const_iterator old(*this);
        if (p)
            p = p->pPrev;
        return old;
    }

    // prefix decrement
    iterator &operator--()
    {
        if (p)
            p = p->pPrev;
        return *this;
    }

private:
    list<T>::Node *p;
    friend class list;
};

/*************************************************
* LIST REVERSE ITERATOR
* Iterate through a List backwards, non-constant version
************************************************/
template <typename T>
class list<T>::const_reverse_iterator
{
public:
    // constructors, destructors, and assignment operator
    const_reverse_iterator() : p(NULL) {}
    const_reverse_iterator(Node *p) : p(p) {}
    const_reverse_iterator(const const_reverse_iterator &rhs) { *this = rhs; }
    const_reverse_iterator &operator=(const const_reverse_iterator &rhs)
    {
        p = rhs.p;
        return *this;
    }

    // equals, not equals operator
    bool operator==(const const_reverse_iterator &rhs) const
    {
        return rhs.p == this->p;
    }
    bool operator!=(const const_reverse_iterator &rhs) const
    {
        return rhs.p != this->p;
    }

    // dereference operator, fetch a node
    T &operator*()
    {
        if (p)
            return p->data;
        else
            throw "ERROR: Trying to dereference a NULL pointer";
    }

    // postfix increment
    const_reverse_iterator operator++(int postfix)
    {
        const_reverse_iterator old(*this);
        if (p)
            p = p->pPrev;
        return old;
    }

    // prefix increment
    const_reverse_iterator &operator++()
    {
        if (p)
            p = p->pPrev;
        return *this;
    }

    // postfix decrement
    const_reverse_iterator operator--(int postfix)
    {
        const_reverse_iterator old(*this);
        if (p)
            p = p->pNext;
        return old;
    }

    // prefix decrement
    const_reverse_iterator &operator--()
    {
        if (p)
            p = p->pNext;
        return *this;
    }

private:
    list<T>::Node *p;

    friend class list;
};

} // namespace custom
#endif