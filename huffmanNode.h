#include <iostream>
#include "minHeap.h"
using namespace std;


  struct node {
    int ascii;
    int count;
    node* left;
    node* right;


      bool operator<(node& rhs)
      {
        return this->count < rhs.count;
        return false;
      }
  };


  ostream& operator<<(ostream& o, node& n)
  {
    o << "(Ascii: " << n.ascii << ", Count: " << n.count << ") "; 

    return o; //returning the completed output to be printed.
  }





  