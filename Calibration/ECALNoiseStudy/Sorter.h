#ifndef Sorter_h
#define Sorter_h

#include <iostream>
#include <vector>

template <typename T>

class Sorter{
  
 public: 

  T sortVar;
  int entry;
  
  Sorter(){};
  ~Sorter(){};

  bool operator() (const Sorter<T> & s1, const Sorter<T> & s2){
    return s1.sortVar < s2.sortVar;
  }
 
};

#endif
