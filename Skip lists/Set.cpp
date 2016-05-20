#include "Set.hpp"

Set::Set(double _q, int _maxLevel) : q(_q), maxLevel(_maxLevel) {
  // Initialization of the head of the skip list with the smallest possible key
  int h = numeric_limits<int>::min();
  head = new SkipNode(h,maxLevel);
  // Initialization of the last element of the list with the largest possible key
  int b = numeric_limits<int>::max();
  back = new SkipNode(b,maxLevel);
  // Connection of all the levels/forward pointers of the header to back
  for(int i = 0; i < head->forward.size(); ++i) {
    head->forward[i] = back;
  }
}

Set::~Set() {
  delete head;
  delete back;
}

Set::Set(const Set& s) {
  *head = *(s.getHead());
  *back = *(s.getBack());
  q = s.getProbability();
  maxLevel = s.getMaxLevel(); 
}

Set& Set::operator=(const Set& s) {
}

void Set::insert(const int& x) {
  SkipNode* n = head;
  cout << "head = " << head->key << endl;
  cout << "Head: " << endl;
  for(int i = 0; i < head->forward.size(); i++) {
    if(head->forward[i])
      cout << head->forward[i]->key << endl;
    else cout << "NULL" << endl;
  }
  cout << "--------" << endl;
  int level = nodeLevel(head->forward);
  cout << "level = " << level << endl;
  vector<SkipNode*> update(maxLevel,NULL);
    cout << "Update: " << endl;
    for(int i = 0; i < update.size(); i++) {
      if(update[i])
	cout << update[i]->key << endl;
      else cout << "NULL" << endl;
    }
    cout << "--------" << endl;
  for(int i = level; i > 0; i--) {
    cout << "i = " << i << endl;
    while(n->forward[i] != NULL and n->forward[i]->key < x) {
      cout << "> forward" << endl;
      n = n->forward[i];
    } 
    update[i] = n;
  }
  cout << "Update after: " << endl;
  for(int i = 0; i < update.size(); i++) {
    if(update[i])
      cout << update[i]->key << endl;
    else cout << "NULL" << endl;
  }
  cout << "--------" << endl;
  n = n->forward[0];
  cout << "n = " << n->key << endl;
  if(n->key != x) {
    int newLevel = randomLevel();
    cout << "newLevel = " << newLevel << endl;
    int currLevel = nodeLevel(update);
    cout << "currLevel = " << currLevel << endl;
    if(newLevel > currLevel) {
      for(int i = currLevel+1; i < newLevel; ++i) {
	update[i] = head;
      }   
    }
    n = new SkipNode(x,newLevel);
    cout << "Update: " << endl;
    for(int i = 0; i < update.size(); i++) {
      if(update[i])
	cout << update[i]->key << endl;
      else cout << "NULL" << endl;
    }
    cout << "--------" << endl;
    // connect pointers of predecessors and new node to successors
    for (int i = 1; i < newLevel; ++i) {
      n->forward[i] = update[i]->forward[i];
      if(n->forward[i])
	cout << "n->forward = " << n->forward[i]->key << endl;
      else cout << "NULL1" << endl;
      update[i]->forward[i] = n;
      if(update[i]->forward[i]) 
	cout << "update->forward = " << update[i]->forward[i]->key << endl;
      else cout << "NULL2" << endl;
    }
  }
}

void Set::remove(const int& x) {
}

int Set::nodeLevel(const vector<SkipNode*>& v) const {
  int currLevel = 1;
  int maxKey = numeric_limits<int>::max();
  if(v[0]->key == maxKey)
    return currLevel;
  for(int i = 0; i < v.size(); ++i) {
    if(v[i] != NULL and v[i]->key != maxKey)
      ++currLevel;
    else break;
  }
  return currLevel;
}

bool Set::contains(const int& x) const {
  SkipNode* n = head;
  int level = nodeLevel(head->forward);
  for(int i = level; i > 0; i--) {
    while(n->forward[i] != NULL and n->forward[i]->key < x) {
      n = n->forward[i];
    }   
  }
  n = n->forward[0];
  if(n->key == x) 
    return true;
  return false;
}

int Set::randomLevel() const {
  int n = 1;
  while ((((double)rand() / RAND_MAX)) < q && abs(n) < maxLevel) 
    n++;
  return abs(n);
}

void Set::print() const {
  SkipNode* list = head;
  int lineLenght = 1;
  cout <<"{" << endl;
  while (list->forward[0] != NULL) {
    cout << "level: "<< nodeLevel(list->forward) << "-> key: "<< list->forward[0]->key <<  endl; 
    list = list->forward[0];
    if(list) cout << "list = " << list->key << endl;
    if(list->forward[0]) cout << "forward = " << list->forward[0]->key << endl;
    //if (list->forward[0] != NULL) cout <<" : ";

    //if (++lineLenght % 2 == 0) cout <<"\n";
  }
  cout <<"}\n";
}

SkipNode* Set::getHead() const {
  return head;
}

SkipNode* Set::getBack() const {
  return back;
}

double Set::getProbability() const {
  return q;
}

int Set::getMaxLevel() const {
  return maxLevel;
}
