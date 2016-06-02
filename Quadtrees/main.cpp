#include "quadtree.hpp"

int main() {
  Quadtree root;
  Point A(0,0);
  Point F(12,8);
  Point B(4,4);
  Point C(10,6);
  Point D(2,7);
  Point E(9,2);
  Point G(20,12);
  Point H(8,11);
  Point I(28,1);
  Point J(-12,6);
  Point K(-20,10);
  Point L(-15,3);
  Point M(-10,-4);
  Point N(16,-8);
  
  root.insert(A);
  root.insert(F);
  root.insert(B);
  root.insert(C);
  root.insert(D);
  root.insert(E);
  root.insert(G);
  root.insert(H);
  root.insert(I);
  root.insert(J);
  root.insert(K);
  root.insert(L);
  root.insert(M);
  root.insert(N);
  
  root.showQuadtree();
  

  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << A << endl << endl;
  root.remove(A);
  root.showQuadtree();
  /*
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p10 << endl << endl;
  root.remove(p10);
  
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p3 << endl << endl;
  root.remove(p3);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p4 << endl << endl;
  root.remove(p4);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p5 << endl << endl;
  root.remove(p5);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p6 << endl << endl;
  root.remove(p6);
  root.showQuadtree();
  cout << endl << endl << "---------" << endl << endl;
  cout << "> removing point " << p10 << endl << endl;
  root.remove(p10);
  root.showQuadtree();
  */
}
