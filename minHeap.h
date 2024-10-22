/*****************************************************
Template prepared by Kazumi Slott
CS311
min heap class
*******************************************************/
#ifndef MINHEAP_H
#define MINHEAP_H
#include <iostream> //for operator<<()
using namespace std;
#include "swap.h" //for mySwap()

template <class T>
class minHeap;

template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h);

template <class T>
class minHeap
{
  friend ostream& operator<< <T>(ostream& o, const minHeap<T>& h);
private:
  T* ar; //dynamic array
  int capacity; //the size of ar
  int num; //the number of elements I have in ar
public:
  minHeap(){ar = NULL; capacity = 0; num = 0;}
  minHeap(int c){capacity = c; ar = new T[c]; num = 0;}
  ~minHeap(){if(ar!=NULL)delete [] ar;}
  void min_heapify(int i);
  //void build_min_heap(); //no need to implement this. We won't use it for our application.
  void bubbleUp(int i);
  void insert(const T& el);
  int find(const T& key) const;
  void remove(int i);
  T getMin();
  T& getElem(int i);
  const T& getElem(int i) const;
  int getCap() const{return capacity;}
  int getNum() const{return num;}
  void updateElem(int i, const T& newValue); //update the element at index i.

  class Overflow{};
  class BadIndex{};
  class NotFound{};
};


//You need to implement all the member functions above.  Don't forget operator<<().

//Prints the current contents of the heap, from root to leaves (smallest to largest).
template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h)
{
  o << "The queue has ";

  if (h.num == 0) {
    return o << "none"; //If the queue has no patients, display "The queue has none"
  }

  for(int i = 0; i < h.num; i++) { //For each patient in the array, add that patient to the output.
    o << h.ar[i] << " "; 
  }

  return o; //returning the completed output to be printed.
}


//turns a heap rooted at i into a min-heap, assuming the left and right subtrees are already min-heaps.
//n is the number of elements in the heap
template <class T>
void minHeap<T>::min_heapify(int i)
{
  int l = 2 * i + 1; //the index of the left child of i
  int r = 2 * i + 2; //the index of the right child of i
  int smallest = i; //the index of the smallest value amoung the values at i, l and r

  //look for the smallest among 3 elements at i, l and r. smallest gets the index of the smallest value.
  //Make sure l and r don't go over the heap's range. 

  if (l < num && ar[l] < ar[smallest]) { //If the left child index does not exceed n, and the value at that index is less than the value at the parent index, then set the left child to the smallest variable.
    smallest = l;
  }
  if (r < num && ar[r] < ar[smallest]) { //If the right child index does not exceed n, and the value at that index is less than the value at the current smallest index, then set the right child to the smallest variable.
    smallest = r;
  }

  // If the smallest is either the right or left child, we need to exchange the parent with it.
  //There might be a violation at the position that was exchanged. Call min_heapify to fix it.
  if (smallest == l) { //If the smallest is the left child, then exchange the parent with the left child, and call min_heapify to make sure the new parent forms a min heap. 
    mySwap(ar[i], ar[l]);
    min_heapify(l);
    }
  if (smallest == r) { //If the smallest is the right child, then exchange the parent with the right child, and call min_heapify to make sure the new parent forms a min heap.
    mySwap(ar[i], ar[r]);
    min_heapify(r);
  }
}


//this function corrects a violiation at i by bubbling it up to the correct place
template <class T>
void minHeap<T>::bubbleUp(int i)
{
  int j = (i - 1) / 2; //j is the index of the parent of i.
  while (i > 0) { //While the child index has not reached the root...
    if (ar[i] < ar[j]) { //if the child node is less than the parent node, then exchange the parent and child nodes.
      mySwap(ar[j], ar[i]);
    }
    i = j; //Move the previous child index to the parent index.
    j = (j - 1) / 2; //Move the previous parent index to its parent index.
  }
}


//Inserts a patient into the min heap.
template <class T>
void minHeap<T>::insert(const T& el)
{
  if(num == capacity) {
    throw Overflow(); //"The array is full"; 
  }
  ar[num] = el; //Insert the element at the end of the array...
  num++;
  int i = num - 1; //i is the index of the patient...
  bubbleUp(i); //Call bubbleUp on the new element to make sure it swaps with the parent(s) if necessary until it's in the correct position.
}


//Find a patient based on their key (patient number).
template <class T>
int minHeap<T>::find(const T& key) const
{
  for(int i = 0; i < num; i++) 
    if(ar[i] == key)
      return i;

  //The element doesn't exist 
  throw NotFound();// "The element doesn't exist";                                                                                        
}


//Remove a patient from the heap, and adjust the heap to maintain the min heap property.
template <class T>
void minHeap<T>::remove(int i)
{
  if (i < 0 || i >= num) { //If the index of the node to be removed is outside the bounds of the array, then throw an error.
    throw BadIndex(); //"The element doesn't exist";
}
  mySwap(ar[i], ar[num - 1]); //Start by exchanging the node to be removed with the last node in the heap.
  num--; //Decrement the number of elements since we're removing one. 
    min_heapify(i); //Call min_heapify to make sure any nodes below the index that was removed are still min heaps.
    bubbleUp(i); //Call bubbledUp to make sure any nodes above the index that was removed are still min heaps.
}


//Removes the highest priority patient (the minimum value, or the root) and returns that patient's number. If the heap is empty, return -1.
template <class T>
T minHeap<T>::getMin()
{
  //This function removes the top element and returns it.
  //You should be calling remove()
  if (num == 0) {
    return T();
  }
  T i = ar[0];
  remove(0);
  return i;
}


template <class T>
T& minHeap<T>::getElem(int i)
{
  if (i < 0 || i >= capacity)
    throw BadIndex();

  return this->ar[i];
}


template <class T>
const T& minHeap<T>::getElem(int i) const
{
  if (i < 0 || i >= capacity)
    throw BadIndex();

  return this->ar[i];
}


template <class T>
void  minHeap<T>::updateElem(int i, const T& newValue)
{
  this->ar[i] = newValue;
  bubbleUp(i);
  min_heapify(i);
}

#endif
