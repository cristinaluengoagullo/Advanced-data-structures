#include <iostream>
#include <vector>
using namespace std;

template <class T>
  
// class SkipNode: it implements a node from the skip list.
class SkipNode {

public:

  // forward has all the pointers that point to the next horizontal level of the list
  // at each height. 
  SkipNode** forward;

  // Constructor, creates an empty node and sets the node parameters key and height 
  // to the given values.
  SkipNode(T key, int height);

  // Constructor, creates an empty node and sets its height to the given value.
  SkipNode(int height);

  // Destructor. 
  ~SkipNode();
 
  // Returns the value of the node.
  T getKey() const;

  // Returns the height of the node.
  int getHeight() const;
 
private:
  // height is the height of the node (number of pointers that point to forward nodes).
  int height;
  // key is the value of the node. 
  T key;
};

template <class T> SkipNode<T>::SkipNode(T _key, int _height) : key(_key), height(_height) {
  // We initialize all forward pointers to null
  forward = new SkipNode<T>* [_height+1];
}

template <class T> SkipNode<T>::SkipNode(int _height) : height(_height) {
  forward = new SkipNode<T>* [_height+1];
}

template <class T> SkipNode<T>::~SkipNode() { 
  delete[] forward;
  /*cout << "entra" << endl;
  int n = forward.size();
  for(int i = 0; i < n; i++) {
    cout << "i = " << i << endl; 
    delete forward[0]; 
    }*/
}

template <class T> T SkipNode<T>::getKey() const {
  return key;
}

template <class T> int SkipNode<T>::getHeight() const {
  return height;
}

