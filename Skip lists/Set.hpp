#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <limits>  
#include <stdlib.h> 
#include "SkipNode.hpp"

template <class T>

class Set {

private:

  SkipNode<T>* head;
  SkipNode<T>* tail;
  float q;
  int maxHeight;
  int currHeight;

public :

  // Constructor, creates an empty Set, sets the skip list 
  // parameter q to the given value.
  Set(double q, int maxHeight); 

  // Destructor.
  ~Set();

  // Copy constructor.
  Set(const Set& s); 

  // Assigment.
  Set<T>& operator=(const Set& s); 

  // Inserts x in the set.
  void insert(const T& x); 

  // Removes x from the set , if present.
  void remove(const T& x); 

  // Returns true iff x is in the set.
  bool contains(const T& x) const; 

  int getNewHeight() const;

  SkipNode<T>* getHead() const;

  SkipNode<T>* getTail() const;

  SkipNode<T>* getProbability() const;

  SkipNode<T>* getMaxHeight() const;

  SkipNode<T>* getCurrHeight() const;

  void print() const;
};

template <class T> Set<T>::Set(double _q, int _maxHeight) : q(_q), maxHeight(_maxHeight), currHeight(1) { 
  // Create head and tail and attach them
  head = new SkipNode<T>(numeric_limits<int>::min(),_maxHeight);
  tail = new SkipNode<T>(numeric_limits<int>::max(),_maxHeight);
  for(int i = 1; i <= _maxHeight; i++)
    head->forward[i] = tail;
}

template <class T> Set<T>::~Set() {
  delete head;
  delete tail;
}

template <class T> Set<T>& Set<T>::operator=(const Set& s) {
  head = s.getHead();
  tail = s.getTail();
  q = s.getProbability();
  maxHeight = s.getMaxHeight();
  currHeight = s.getCurrHeight();
}

template <class T> void Set<T>::insert(const T& x) {
  SkipNode<T>** update = new SkipNode<T>* [maxHeight+1];
  SkipNode<T>* tmp = head;
  // Figure out where new node goes
  for(int h = currHeight; h > 0; h--) {
    while (tmp->forward[h]->getKey() < x) {
	tmp = tmp->forward[h];
      }
    update[h] = tmp;
  }
  tmp = tmp->forward[1];
  // If dup, return false
  if (tmp->getKey() != x) {
    // Perform an insert
    int lvl = getNewHeight();
    if(lvl > currHeight) {
      for(int i = currHeight+1; i <= lvl; i++)
	update[i] = head;
      currHeight = lvl;
    }
    // Insert new element
    tmp = new SkipNode<T>(x,lvl);
    for(int i = 1; i <= lvl; i++) {
      tmp->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = tmp;
    }
  }
}

template <class T> void Set<T>::remove(const T& x) {
  SkipNode<T>** update = new SkipNode<T>*[maxHeight+1];
  SkipNode<T>* tmp = head;
  // Find the node we need to delete
  for(int i = currHeight; i > 0; i--) {
    while(tmp->forward[i]->getKey() < x) {
      tmp = tmp->forward[i];
    }
    update[i] = tmp;
  }
  tmp = tmp->forward[1];
  if(tmp->getKey() == x) {
    for(int i = 1; i <= currHeight; i++) {
      if(update[i]->forward[i] != tmp) 
	break;
      update[i]->forward[i] = tmp->forward[i];
    }
    delete tmp;
    while((currHeight > 1 ) and ((head->forward[currHeight]->getKey() == tail->getKey())))
      currHeight--;
  }
}

template <class T> bool Set<T>::contains(const T& x) const {
  SkipNode<T>* tmp = head; 
  // Find the key and return the node
  for(int i = currHeight; i > 0; i--) {
    while(tmp->forward[i]->getKey() < x ) {
	tmp = tmp->forward[i];
    }
  }
  tmp = tmp->forward[1];
  if (tmp->getKey() == x) return true;
  return false;
}

template <class T> int Set<T>::getNewHeight() const {
  int tmpLvl = 1;
  // Develop a random number between 1 and
  // maxLvl (node height).
  while((((double)rand() / RAND_MAX)) < q and abs(tmpLvl) < maxHeight) 
    tmpLvl++;
  return tmpLvl;
}

template <class T> SkipNode<T>* Set<T>::getHead() const {
  return head;
}

template <class T> SkipNode<T>* Set<T>::getTail() const {
  return tail;
}

template <class T> SkipNode<T>* Set<T>::getProbability() const {
  return q;
}

template <class T> SkipNode<T>* Set<T>::getMaxHeight() const {
  return maxHeight;
}

template <class T> SkipNode<T>* Set<T>::getCurrHeight() const {
  return currHeight;
}

template <class T> void Set<T>::print() const {
  SkipNode<T>* tmp;
  tmp = head;
  while(tmp != tail) {
    if(tmp == head)
      cout << "There's the head node!" << endl;
    else
      cout << "Next node holds key: " << tmp->getKey() << " (" << tmp->getHeight() << ")" << endl;
    tmp = tmp->forward[1];
  }
  cout << "There's the tail node!" << endl;
}

#endif
