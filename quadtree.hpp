#include <vector>
#include <iostream>
#include <climits>
#include <cmath>  
#include <string>  
using namespace std;

const static int NE = 1;
const static int NW = 2;
const static int SW = 3;
const static int SE = 4;

struct Point {
  int x, y;
  Point() : x(0), y(0) {}
  Point(int _x, int _y) : x(_x), y(_y) {}
  bool operator==(const Point p) const {
    return (x == p.x and y == p.y);
  } 
  friend ostream& operator<<(ostream& os, const Point& p) {
    os << "(" << p.x << "," << p.y << ")"; 
    return os;
  } 
};

struct QuadtreeNode {
  vector<QuadtreeNode*> quadrants;
  Point point;

  QuadtreeNode() : quadrants(vector<QuadtreeNode*>(4,NULL)) {}

  void clone(QuadtreeNode* n) {
    if(n) {
      point = n->point;
      for(int i = 0; i < 4; i++) {
	if(n->quadrants[i]) {
	  quadrants[i] = new QuadtreeNode;
	  quadrants[i]->clone(n->quadrants[i]);
	}
      }
    }
  }

  void showQuadtreeNode(string spaces, string quadtree) {
    cout << spaces << quadtree << " (" << point.x << "," << point.y << ")" << endl;
    if(quadrants[NE-1]) {
      quadrants[NE-1]->showQuadtreeNode(spaces+"  ","NE:");
    }
    if(quadrants[NW-1]) {
      quadrants[NW-1]->showQuadtreeNode(spaces+"  ","NW:");
    }
    if(quadrants[SW-1]) {
      quadrants[SW-1]->showQuadtreeNode(spaces+"  ","SW:");
    }
    if(quadrants[SE-1]) {
      quadrants[SE-1]->showQuadtreeNode(spaces+"  ","SE:");
    }
  }

  void deleteNode() {
    for(int i = 0; i < 4; i++) {
      if(quadrants[i]) 
	quadrants[i]->deleteNode();
    }
    for(int i = 0; i < 4; i++) {
      delete quadrants[i];
    }
  }
};

class Quadtree {

private:
  
  QuadtreeNode* root;
  
  vector<Point> points;

  int LP,RP,BP,TP;
  
  int compare(const Point& p, QuadtreeNode* node);

  int conjugate(int n);

  QuadtreeNode* findCandidate(QuadtreeNode* node, int quadrant);
  
  bool insert(QuadtreeNode* node);

  bool isInCrossSection(QuadtreeNode* quadrantRoot, QuadtreeNode* deletedNode, QuadtreeNode* candidate) const;

  void ADJ(QuadtreeNode* quadrantRoot, int quadrantId, QuadtreeNode* deletedNode, QuadtreeNode* candidate, Quadtree& newQuadtree);

  bool search_rec(QuadtreeNode* node, const Point& p);
  
  QuadtreeNode* searchNode(QuadtreeNode* node, const Point& p);

  bool in_region(const Point& p);

  bool rectangle_overlaps_region(int L, int R, int B, int T);

  void regionsearch_rec(QuadtreeNode* node, int L, int R, int B, int T);

public:

  Quadtree();

  Quadtree(const Quadtree& q);
  
  ~Quadtree();

  bool insert(const Point& p);

  void search(const Point& p);

  void regionsearch(int L, int R, int B, int T);

  void remove(const Point& p);

  QuadtreeNode* getRoot() const;

  void showQuadtree() const;
};
