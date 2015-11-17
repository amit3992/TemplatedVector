// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Amit Kulkarni

#include <iostream> // debugging
#include <stdlib.h>
#include <sstream>

#include "Vector.h"
#include "String.h"


// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
  // Default constructor constructs an empty vector.
  // Initializing all private variables mentioned in Vector.h
  elements = NULL;
  count = 0;
  reserved = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
  // Assign global variables
  count = rhs.count;
  reserved = rhs.reserved;

  // Allocate 'reserved' amount of memory using malloc
  elements = (T*)malloc(reserved * sizeof(T));

  // Fill the allocated memory with 'count' no. of elements using in-place New
  for(int i = 0; i < count; i++)
  {
      new(&elements[i])T(rhs.elements[i]);
  }

}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  // 1] Check if it is pointing to itself. If true, Return
  // 2] Check if current elements are null. If not, Call destructor on current elements and free respective heap mem.
  // 3] Copy new variables and Allocate new memory and call in-place new to create 'count' new objects.

  // Step 1:
  if (this ==  &rhs)
      return *this;

  // Step 2:
  if (elements != NULL)
  {
    for(int i = 0; i < count; i++)
    {
      elements[i].~T();
    }
    free(elements);
  }

  // Step 3:
  count = rhs.count;
  reserved = rhs.reserved;

  elements = (T*)malloc(reserved * sizeof(T));  // Allocating memory

  for(int i = 0; i < count; i++)
  {
    new(&elements[i])T(rhs.elements[i]); // Copy elements to heap
  }
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
  // Use malloc not 'new'. New creates objects, we just want memory.
  elements = (T*)malloc(nReserved * sizeof(T));
  count = 0;
  reserved = nReserved;
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
  // Use 'malloc' here as well.
  // in-place new. 'new(&elements[i])T(t)'
  count  = n; // No.of copies of the object
  reserved = n;
  elements = (T*)malloc(count * sizeof(T));

  // Call in-place new to create 'n' copies in the allocated memory
  for(int i = 0; i < count; i++)
  {
    new(&elements[i])T(t);
  }

}

// Reserve method definition.
template <typename T>
void Vector<T>::Reserve(size_t n)
{
  // Using malloc to allocate 'n' amount of memory in heap.
  this->reserved = n;
  elements = (T*)malloc(n * sizeof(T));
}
#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  // Clear vector and free all the memory.
  if(elements) // Destroy only if elements exist
  {
    for(int i = 0; i < count; i++)
    {
      elements[i].~T();
    }
    free(elements);
  }
  // Set count and reserved to 0.
  reserved = 0;
  count = 0;
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
  // 1] If count = reserverd, Create a tempVec of size 'reserved+1'.
  // 2] Copy old elements in new tempVec. Call destructor on elements and release
  // 3] Copy new element in the last position

  // Step 1:
  if(count == reserved)
  {
    reserved = reserved + 1;
    T* tempVec = (T*)malloc(reserved * sizeof(T));
    count = count + 1;

    // Step 2:
    for(int i = 0; i < (count-1); i++)      // --> Segmentation fault
    {
      new(&tempVec[i])T(elements[i]);
      elements[i].~T();
    }

    // Step 3:
    new(tempVec+(count-1))T(rhs);

    // Release memory
    free(elements);

    // Copy back from tempVec to elements
    elements = tempVec;

  }
  else
  {
    // If count < reserved => Space is available
    // Just copy the new Element
    count++;
    new(&elements[count-1])T(rhs);
  }
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
  // Same algorithm as Push_Back.
  // Copy new element in 0th position

  // Step 1:
  if(count == reserved)
  {
    reserved = reserved + 1;
    T* tempVec = (T*) malloc(reserved * sizeof(T));
    count = count + 1;

    // Step 2:
    for(int i = 1; i < count; i++)      // --> Segmentation fault
    {
      new(&tempVec[i])T(elements[i-1]);
      elements[i-1].~T();
    }

    // Step 3:
    new(tempVec)T(rhs);      // --> check location

    // Release memory
    free(elements);

    // Copy back from tempVec to elements
    elements = tempVec;
  }
  else
  {
      // Shift elements to right by one position
      // Copy new element to the 0th position
      for(int i = count; i > count; i--)
      {
        new(&elements[i])T(elements[i-1]);
        elements[i-1].~T();
      }
      new(elements)T(rhs);
      count++;
  }

}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element and destroy. Does not Return
  if(count != 0)    // Only if elements are not empty
  {
    count = count - 1;
    elements[count].~T();
  }

}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
  // 1] Check if elements is not empty. If not, copy elements[1] to count to tempVec
  // 2] Delete elements[0]
  // 3] Copy all elements back from tempVec. Decrement count.

  // Step 1:
  if(count != 0)    // Only if elements are not empty
  {
    // Step 1:
    T* tempVec = (T*)malloc(reserved * sizeof(T));
    for(int i = 1; i < count; i++)
    {
      new(&tempVec[i-1])T(elements[i]);
      elements[i].~T();
    }

    // Step 2:
    elements[0].~T();
    free(elements);

    //Step 3:
    elements = tempVec;
    count = count - 1;
  }
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  // return the 0th element
  return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  // Return the last element
  int last = count - 1;
  return elements[last];
}

// Element access.
template <typename T>
T& Vector<T>::operator[](size_t i) const
{
  if (i < 0 || i >= count)
  {
    cout << "ERROR INDEXING!" << endl;
    abort();
  }
  return (*(elements+i));
}


template <typename T>
size_t Vector<T>::Size() const
{
  // Vector count signifies the current size of the vector
  return count;
}

template <typename T>
bool Vector<T>::Empty() const
{

  if (count == 0)
      return true;
  else
      return false;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  // While not empty, pop_back or Loop through the vector and call destructor
  // Reset count. DO NOT CLEAR MEMORY

  for(int i = 0; i < count; i++)
  {
    elements[i].~T();     // Call destructor for each element
  }

  count  = 0;   // Reset count to 0

}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  // Points to next element
  return VectorIterator<T>(elements+count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
  //cout << "Erase begin" << endl;
  // 1] Loop through the elements to locate the element
  // 2] If the element is found, erase it by overwriting elements after it.
  bool flag = false;
  int pos; // Since it can only be positive

  for(int i = 0; i < count; i++)
  {
    if((&elements[i]) == it.current)
    {
      pos = i;
      flag = true;
      break;
    }
  }

  if(flag == true)
  {
    for(int i = pos; i < count-1; i++)
    {
      // First call destructor and then copy
      elements[i].~T();
      new(&elements[i])T(elements[i+1]); // --> check
    }
  }

//  cout << "Erase end" << endl;

}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
//  cout << "Insert begin" << endl;
  // 1] Loop through elements to locate the position
  int pos;
  bool flag = false;

  for(int i = 0; i < count; i++)
  {
    if((&elements[i]) == it.current)
    {
      flag = true;
      pos = i;
      break;
    }
  }

  // 1] If count = reserved, create new 'tempVec' of size 'reserved+1'
  // 2] Copy all elements lef of 'pos' to tempVec
  // 3] Copy 'rhs' in 'pos'
  // 4] Copy all elements right of pos to tempVec
  if(flag == true)
  {
    // Step 1:
    if(count == reserved)
    {
      reserved = reserved + 1;
      T* tempVec = (T*)malloc(reserved * sizeof(T));

      // Step 2:
      for(int i = 0; i < pos; i++)
      {
        new(&tempVec[i])T(elements[i]);
        elements[i].~T();
      }

      // Step 3:
      new(tempVec+pos)T(rhs);

      // Step 4:
      for(int i = count; i > pos; i--)
      {
        new(&tempVec[i])T(elements[i-1]);
        elements[i-1].~T();
      }
      count++;
      free(elements);

      // Copy back data from tempVec
      elements = tempVec;


    }
    else      // count < reserved
    {
      // Shift right elements to the right of pos by 1
      // Copy new element in pos
      // increment count
      for(int i = count; i > pos; i--)
      {
        // Step 1:
        new(&elements[i])T(elements[i-1]);
        elements[i-1].~T();
      }
      // Step 2:
      elements[pos].~T();
      new(&elements[pos])T(rhs);
      // Step 3:
      count++;
    }
  }

//  cout << "Insert finish" << endl;
}
#endif

// Implement the iterators
// Iterator is not a pointer, its a class.

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator() // Default
{
  current = NULL;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
  // Assign c to current
  current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
  current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;  // De-referencing operator
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
  // Prefix returns the incremented value
  // Increment value of current and then return
  current = current + 1;
  return *this;

}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{
  // Postfix returns the original value prior to increment
  // Copy original value in temp(old), increment it and return original value
  VectorIterator<T> old = *this;
  current = current + 1;
  return old;

}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
  // Return true if current != rhs.current
  if (current != rhs.current)
  {
    return true;
  }
  else
  {
    return false;
  }
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
  // Return true if current == rhs.current
  if (current == rhs.current)
  {
    return true;
  }
  else
  {
    return false;
  }
}
