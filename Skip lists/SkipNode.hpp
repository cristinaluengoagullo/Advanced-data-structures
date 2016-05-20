#include <iostream>
#include <vector>
using namespace std;

template <class T>
  
class SkipNode {

public:

  vector<SkipNode*> forward;

  SkipNode(T key, int height);

  SkipNode(int height);

  ~SkipNode();
 
  T getKey() const;

  int getHeight() const;
 
private:

  int height;
  T key;
};

template <class T> SkipNode<T>::SkipNode(T _key, int _height) : key(_key), height(_height) {
  forward = vector<SkipNode*>(_height+1,NULL);
}

template <class T> SkipNode<T>::SkipNode(int _height) : height(_height) {
  forward = vector<SkipNode*>(_height+1,NULL);
}

template <class T> SkipNode<T>::~SkipNode() {
  /*if(forward.size()) {
    cout << forward.size() << endl;
    for(int i = 0; i <= forward.size(); i++) {
      cout << "i = " << i << endl;
      delete (forward[i]);
    }
    }*/
}

template <class T> T SkipNode<T>::getKey() const {
  return key;
}

template <class T> int SkipNode<T>::getHeight() const {
  return height;
}

