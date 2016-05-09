#include "quadtree.hpp"

int main() {
  Quadtree root;
  Point p1(0,0);
  Point p2(2,1);
  Point p3(2,-2);
  Point p4(-3,4);
  Point p5(-2,6);
  Point p6(4,2);
  Point p7(-3,-2);
  Point p8(3,3);
  Point p9(0,2);
  Point p10(1,-1);
  
  root.insert(p1);
  root.insert(p2);
  root.insert(p3);
  root.insert(p4);
  root.insert(p5);
  root.insert(p6);
  root.insert(p7);
  root.insert(p8);
  root.insert(p9);
  root.insert(p10);
  
  root.search(p1);
  root.search(p2);
  root.search(p3);
  root.search(p4);
  root.search(p5);
  root.search(p6);
  root.search(p7);
  root.search(p8);
  root.search(p9);
  
  root.regionsearch(-10,10,-10,10);
  root.regionsearch(0,10,0,10);
  root.regionsearch(-2,2,-2,2);
 
  cout << endl << "---------" << endl << endl;
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p1 << endl << endl;
  root.remove(p1);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p2 << endl << endl;
  root.remove(p2);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p3 << endl << endl;
  root.remove(p3);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
}
