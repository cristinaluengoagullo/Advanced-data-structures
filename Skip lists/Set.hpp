#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <limits>  
#include <stdlib.h> 
#include <ctime>
#include "SkipNode.hpp"

template <class T>

// Set class: implements a skip list.
class Set {

public :

  // Constructor, creates an empty Set and sets the skip list parameters q and 
  // maxHeight to the given values.
  Set(double q, int maxHeight); 

  // Destructor.
  ~Set();

  // Copy constructor.
  Set(const Set& s); 

  // Assigment.
  Set<T>& operator=(const Set& s); 

  // Inserts x in the set.
  void insert(const T& x); 

  // Removes x from the set, if present.
  void remove(const T& x); 

  // Returns true if and only if x is in the set.
  bool contains(const T& x) const; 

  // Returns the height at which an element will be inserted. 
  int getNewHeight() const;

  // Returns the head of the skip list.
  SkipNode<T>* getHead() const;

  // Returns the tail of the skip list.
  SkipNode<T>* getTail() const;

  // Returns the probability of height in a skip list.
  SkipNode<T>* getProbability() const;

  // Returns the maximum height of the skip list.
  SkipNode<T>* getMaxHeight() const;

  // Returns the current height of the skip list.
  SkipNode<T>* getCurrHeight() const;

  // Returns the total search cost in the list.
  int totalSearchCost(int l, int r) const;
  
  // Returns the number of pointers used in order to implement the structure.
  int numberPointers() const;

  // Prints the skip list in such a way that both the nodes values and 
  // their heights are visible.
  void print() const;

private:

  // head denotes the head of the skip list (it has the minimum integer value).
  SkipNode<T>* head;
  // head denotes the head of the skip list (it has the maximum integer value).
  SkipNode<T>* tail;
  // q defines the probability of a node to be inserted at a certain height in the skip list. 
  float q;
  // maxHeight is the maximum height the list can have.
  int maxHeight;
  // currHeight is the height of the skip list at a certain moment. 
  int currHeight;
};

template <class T> Set<T>::Set(double _q, int _maxHeight) : q(_q), maxHeight(_maxHeight), currHeight(1) { 
  // Creation of head and tail.
  head = new SkipNode<T>(numeric_limits<int>::min(),_maxHeight);
  tail = new SkipNode<T>(numeric_limits<int>::max(),_maxHeight);
  for(int i = 1; i <= _maxHeight; i++)
    // We attach head to tail at every vertical level.
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
  // update keeps the pointers that point to the nodes at which the inserted element will have to point
  // once inserted. 
  SkipNode<T>** update = new SkipNode<T>* [maxHeight+1];
  SkipNode<T>* tmp = head;
  // We first figure out where the element will be inserted. 
  for(int h = currHeight; h > 0; h--) {
    while(tmp->forward[h]->getKey() < x)
	tmp = tmp->forward[h];
    update[h] = tmp;
  }
  tmp = tmp->forward[1];
  // If this condition does not apply, it means that the element already exists in 
  // the skip list. 
  if (tmp->getKey() != x) {
    // We figure out the height of the node to insert.
    int lvl = getNewHeight();
    // If the height at which the node will be inserted is greater than the current
    // height of the skip list, we update the current height and the update pointers.
    if(lvl > currHeight) {
      for(int i = currHeight+1; i <= lvl; i++)
	update[i] = head;
      currHeight = lvl;
    }
    // We create a new node and insert it.
    tmp = new SkipNode<T>(x,lvl);
    for(int i = 1; i <= lvl; i++) {
      tmp->forward[i] = update[i]->forward[i];
      update[i]->forward[i] = tmp;
    }
  }
}

template <class T> void Set<T>::remove(const T& x) {
  // update keeps the pointers that point to the nodes at which the previous element to 
  // the one removed will have to point.
  SkipNode<T>** update = new SkipNode<T>*[maxHeight+1];
  SkipNode<T>* tmp = head;
  // We first figure out where the element is. 
  for(int i = currHeight; i > 0; i--) {
    while(tmp->forward[i]->getKey() < x)
      tmp = tmp->forward[i];
    update[i] = tmp;
  }
  tmp = tmp->forward[1];
  // If we find the element in the skip list, we delete it.
  if(tmp->getKey() == x) {
    for(int i = 1; i <= currHeight; i++) {
      if(update[i]->forward[i] != tmp) 
	break;
      update[i]->forward[i] = tmp->forward[i];
    }
    delete tmp;
    // We update the value of the current height if its value changed with
    // the deletion of the element. 
    while((currHeight > 1 ) and ((head->forward[currHeight]->getKey() == tail->getKey())))
      currHeight--;
  }
  //delete[] update;
  //delete tmp;
}

template <class T> bool Set<T>::contains(const T& x) const {
  SkipNode<T>* tmp = head; 
  // We figure out where the element is, if it exists. 
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
  // We randomly decide based on the probability of the skip list the height
  // of a node to be inserted. 
  while(((double)(rand())/RAND_MAX < q) and tmpLvl < maxHeight) {
    tmpLvl++;
  }
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

template <class T> int Set<T>::totalSearchCost(int l, int r) const {
  if(l > r) return 0;
  if(l == r) return 1;
  SkipNode<T>* tmp = head;
  SkipNode<T>* maxNode;
  int maxHeight = 0;
  // We start inspecting the skip list from the right place.
  for(int i = 0; i < l; i++) 
    tmp = tmp->forward[1];
  // Compute the first node that has height equal to currHeight (the highest 
  // height of the list, not counting the head or the tail).
  for(int i = l; i < r; i++) {    
    if(maxHeight < tmp->getHeight()) {
      maxNode = tmp;
      maxHeight = tmp->getHeight();
    }
    tmp = tmp->forward[1];
  }
  return (r-l) + 1 + totalSearchCost(l,maxNode->getKey()-1) + totalSearchCost(maxNode->getKey()+1,r);
}

template <class T> int Set<T>::numberPointers() const {
  SkipNode<T>* tmp;
  int sum = 0;
  tmp = head;
  // We compute the total sum of pointers used in order to implement the structure by summing
  // the heights of the different nodes. 
  while(tmp != tail) {
    sum += tmp->getHeight();
    tmp = tmp->forward[1];
  }
  return sum;
}

template <class T> void Set<T>::print() const {
  SkipNode<T>* tmp;
  tmp = head;
  // While we are not either in the head or the tail of the skip list, we print the information
  // of the different nodes contained in the structure. 
  while(tmp != tail) {
    cout << "Next node holds key: " << tmp->getKey() << " (" << tmp->getHeight() << ")" << endl;
    tmp = tmp->forward[1];
  }
}

#endif
