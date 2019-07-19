/***********************************************************************
* Module:
* Week 03, Quue
* Brother Helfrich, CS 235
* Author:
* Br. Helfrich
* Summary:
* This program will implement a queue
************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <cassert> // because I am paranoid

namespace custom
{

/**************************************************
* queue
* First-in-First-out data structure
*************************************************/
template <class T>
class queue
{
public:
// constructors, destructors, and assignment operator
queue() : data(NULL), numCapacity(0), numPush(0), numPop(0) { }
queue(int max) throw(const char *);
queue(const queue <T> & rhs) throw(const char *);
~queue() { if (numCapacity != 0) delete [] data; }
queue <T> & operator = (const queue <T> & rhs) throw (const char *);

// standard container interfaces
int size () const { return numPush - numPop; }
bool empty() const { return size() == 0; }
void clear() { numPush = numPop = 0; }

// queue-specific interfaces
void push(const T & t) throw(const char *);
void pop();
T & front() throw(const char *);
T & back() throw(const char *);

private:
// grow the queue as necessary. This will double the size
void resize(int newCapacity = 0) throw(const char *);

// find the index of the head or the tail in the data array
int iTail() const
{
assert(numCapacity != 0);
return (numPush - 1) % numCapacity;
}
int iHead() const
{
assert(numCapacity != 0);
return numPop % numCapacity;
}

// member variables
T * data; // dynamically allocated data for the queue
int numCapacity; // the size of the data array
int numPush; // number of items ever pushed to the queue
int numPop; // number of items ever popped from the queue

void display() const;

};

#ifdef DEBUG
using std::cerr;
using std::endl;
/*******************************************************
* debug display
*******************************************************/
template <class T>
void queue <T> :: display() const
{
cerr << "data = ";
for (int i = 0; i < numCapacity; i++)
{
if (iHead() >= iTail() && (i <= iTail() || i >= iHead()))
cerr << data[i] << ' ';
else if (iHead() <= iTail() && i >= iHead() && i <= iTail())
cerr << data[i] << ' ';
else
cerr << "? ";
}
cerr << endl;

cerr << "numPush " << numPush << endl;
cerr << "numPop " << numPop << endl;
cerr << "numCapacity " << numCapacity << endl;

if (numCapacity)
{
cerr << "iHead() " << iHead() << endl;
cerr << "iTail() " << iTail() << endl;
}
}
#else
template <class T>
void queue <T> :: display() const
{
}
#endif // DEBUG

/********************************************************
* QUEUE :: copy constructor
*******************************************************/
template <class T>
queue <T> & queue <T> :: operator = (const queue <T> & rhs) throw(const char *)
{
// do nothing if there is nothing to do
if (&rhs == this)
return *this;

// forget about anything that was there before
clear();

// an empty RHS then set numPush and numPop to nothing
if (rhs.size() == 0)
return *this;

assert(rhs.numCapacity > 0);
assert(rhs.numPush > 0 && rhs.numPop >= 0);

// create/reallocate a buffer if needed.
if (rhs.size() > numCapacity)
resize(rhs.size());

// copy the data
for (int iSource = rhs.numPop; iSource < rhs.numPush; iSource++)
data[numPush++] = rhs.data[iSource % rhs.numCapacity];
assert(numPush <= numCapacity);

return *this;
}

/**********************************************************
* QUEUE :: non-default constructor
*********************************************************/
template <class T>
queue <T> :: queue(int newCapacity) throw(const char *) :
numPush(0), numPop(0), numCapacity(0), data(NULL)
{
if (newCapacity > 0)
resize(newCapacity);
}

/**********************************************************
* QUEUE :: non-default constructor
*********************************************************/
template <class T>
queue <T> :: queue(const queue <T> & rhs) throw(const char *) :
numPush(0), numPop(0), numCapacity(0), data(NULL)
{
*this = rhs;
}

/*********************************************************
* QUEUE :: PUSH
* Add an item to the queue
********************************************************/
template <class T>
void queue <T> :: push(const T & t) throw (const char *)
{
display();

// grow if the queue is not currently large enough
if (size() == numCapacity)
resize();

// add it to the tail position
numPush++;
data[iTail()] = t;
}

/*********************************************************
* QUEUE :: POP
* Remove an item from the end of the queue
********************************************************/
template <class T>
void queue <T> :: pop()
{
// do nothing if the queue is currently empty
if (empty())
return;

// forget about the last item
numPop++;
}

/********************************************************
* QUEUE :: FRONT
* return a reference to the next element in the front of the queue
********************************************************/
template <class T>
T & queue <T> :: front() throw (const char *)
{
// determine if the queue is empty
if (empty())
throw "ERROR: attempting to access an element in an empty queue";

// otherwise, return the answer
return data[iHead()];
}

/********************************************************
* QUEUE :: BACK
* return a reference to the last element in the back of the queue
********************************************************/
template <class T>
T & queue <T> :: back() throw (const char *)
{
// determine if the queue is empty
if (empty())
throw "ERROR: attempting to access an element in an empty queue";

// otherwise, return the answer
return data[iTail()];
}

/*********************************************************
* QUEUE :: RESIZE
* Grow the size of the queue to (at a minimum) of a given
* size. If the size is set to zero, then the capacity will
* be doubled. If the stack is currently not empty, the contents
* will be copied over to the new buffer
*********************************************************/
template <class T>
void queue <T> :: resize(int newCapacity) throw(const char *)
{
assert(newCapacity >= 0);

// determine the new buffer size
if (newCapacity == 0)
newCapacity = (numCapacity != 0 ? numCapacity * 2 : 1);

// allocate the new buffer
T * newData = new(std::nothrow) T[newCapacity];
if (NULL == newData)
throw "ERROR: Unable to allocate a new buffer for queue";

// copy the data
int iDestination = 0;
for (int iSource = numPop; iSource < numPush; iSource++)
newData[iDestination++] = data[iSource % numCapacity];

// set the new capacity, numPush, and numPop
numCapacity = newCapacity;
numPop = 0;
numPush = iDestination;

// free the old
if (NULL != data)
delete [] data;
data = newData;
}
}

#endif // QUEUE_H