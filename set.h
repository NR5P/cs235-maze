/***********************************************************************
* Header:
* Set
* Summary:
* This will contain the class definition of:
* set : A class that represents a Set
* set::iterator : An interator through set
* Author
* Br. Helfrich
************************************************************************/

#ifndef SET_H
#define SET_H

#include <cassert>
#include <iostream>


#include "bst.h"

namespace custom
{

/************************************************
* SET
* A class that represents a Set
***********************************************/
template <class T>
class set
{
public:
// constructors, destructors, and assignment operator
set() : bst() {}
set(int capacity) : bst() {}
set(const set & rhs) : bst(rhs.bst) {}
set & operator=(const set & rhs)
{
bst = rhs.bst;
return *this;
}

// standard interfaces
int size() const { return bst.size(); }
bool empty() const { return size() == 0; }
void clear() { bst.clear(); }

// iterator
class iterator;
iterator begin() { return iterator(bst.begin()); }
iterator end() { return iterator(bst.end()); }

class const_iterator;
const_iterator cbegin() const { return const_iterator(bst.cbegin()); }
const_iterator cend() const { return const_iterator(bst.cend()); }

// set-specific stuff
void insert(const T & t) throw (const char *)
{
if (bst.find(t) == bst.end())
bst.insert(t);
}
iterator find(const T & t)
{
return iterator(bst.find(t));
}
void erase(iterator & it)
{
bst.erase(it.it);
}

// union, intersection, etc.
set <T> operator || (const set <T> & rhs) const throw (const char *);
set <T> operator && (const set <T> & rhs) const throw (const char *);
set <T> operator - (const set <T> & rhs) const throw (const char *);

private:
BST <T> bst;
};

/****************************
* ITERATOR
****************************/
template <class T>
class set <T> :: iterator
{
public:
// constructors
iterator() : it() {}
iterator(const iterator & rhs) : it(rhs.it) {}
iterator(const typename BST <T> :: iterator & rhs) : it(rhs) {}
iterator & operator=(const iterator & rhs)
{
it = rhs.it;
return *this;
}

// increment and decrement
iterator & operator++()
{
++it;
return *this;
}
iterator & operator--()
{
--it;
return *this;
}
iterator operator++(int postfix)
{
iterator tmp(it);
++(*this);
return tmp;
}
iterator operator--(int postfix)
{
iterator tmp(it);
--(*this);
return tmp;
}

// dereference operator
T operator*() const { return *it; }

// comparisons
bool operator==(const iterator & rhs) const { return it == rhs.it; }
bool operator!=(const iterator & rhs) const { return !(*this == rhs); }

friend void set <T> :: erase(set <T> :: iterator & it);

private:
typename BST <T> :: iterator it;
};

/****************************
* CONST ITERATOR
****************************/
template <class T>
class set <T> :: const_iterator
{
public:
// constructors
const_iterator() : it() {}
const_iterator(const iterator & rhs) : it(rhs.it) {}
const_iterator(const typename BST <T> :: const_iterator & rhs) : it(rhs) {}
const_iterator & operator=(const const_iterator & rhs)
{
it = rhs.it;
return *this;
}

// increment and decrement
const_iterator & operator++()
{
++it;
return *this;
}
const_iterator & operator--()
{
--it;
return *this;
}
const_iterator operator++(int postfix)
{
const_iterator tmp(it);
++(*this);
return tmp;
}
const_iterator operator--(int postfix)
{
const_iterator tmp(it);
--(*this);
return tmp;
}

// dereference operator
T operator*() const { return *it; }

// comparisons
bool operator==(const const_iterator & rhs) const {return it == rhs.it; }
bool operator!=(const const_iterator & rhs) const {return !(*this == rhs); }

private:
typename BST <T> :: const_iterator it;
};

/**************************************
* UNION
**************************************/
template <class T>
set <T> set<T> :: operator || (const set <T> & rhs) const throw (const char *)
{
set <T> setReturn;

set <T> :: const_iterator itLhs = cbegin();
set <T> :: const_iterator itRhs = rhs.cbegin();

while (itLhs != cend() || itRhs != rhs.cend())
{
if (itLhs == cend())
setReturn.insert(*(itRhs++));
else if (itRhs == rhs.cend())
setReturn.insert(*(itLhs++));
else if (*itLhs == *itRhs)
{
setReturn.insert(*itLhs);
++itLhs;
++itRhs;
} else if (*itLhs < *itRhs)
setReturn.insert(*(itLhs++));
else
setReturn.insert(*(itRhs++));
}

return setReturn;
}


/**************************************
* INTERSECTION
**************************************/
template <class T>
set <T> set<T> :: operator && (const set <T> & rhs) const throw (const char *)
{
set <T> setReturn;

set <T> :: const_iterator itLhs = cbegin();
set <T> :: const_iterator itRhs = rhs.cbegin();

while (itLhs != cend() || itRhs != rhs.cend())
{
if (itLhs == cend())
return setReturn;
else if (itRhs == rhs.cend())
return setReturn;
else if (*itLhs == *itRhs)
{
setReturn.insert(*itLhs);
++itLhs;
++itRhs;
} else if (*itLhs < *itRhs)
itLhs++;
else
itRhs++;
}

return setReturn;
}

/***********************************************
* SET : DIFFERENCE
* Create a set consisting of the elements in
* this but not in rhs
* COST: O(n + m)
***********************************************/
template <class T>
set <T> set <T> :: operator - (const set <T> & rhs) const throw (const char *)
{
set <T> setReturn;

set <T> :: const_iterator itLhs = cbegin();
set <T> :: const_iterator itRhs = rhs.cbegin();

// continue looping as long as there are items in either list
while (itLhs != cend() || itRhs != rhs.cend())
{
// if we are at the end of the LHS, then skip the RHS
if (itLhs == cend())
++itRhs;
// if we are at the end of the RHS, add everything left on RHS
else if (itRhs == rhs.cend())
setReturn.insert(*(itLhs++));
// if they are the same, skip it
else if (*itLhs == *itRhs)
{
++itLhs;
++itRhs;
}
// if RHS is smaller we can ignore it, then skip iRhs
else if (*itRhs < *itLhs)
++itRhs;
// otherwise LHS is smaller so we can add it
else
{
setReturn.insert(*(itLhs++));
}
}
return setReturn;
}

 

}
#endif 