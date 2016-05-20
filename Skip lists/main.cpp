#include "Set.hpp"

int main() {
  Set<int> s(0.5,5);
  s.insert(1);
  s.insert(2);
  s.insert(3);
  s.insert(6);
  s.insert(4);
  s.print();
  if(s.contains(2)) cout << "It contains 2!" << endl;
  else cout << "It doesn't contain 2!" << endl;
  s.remove(2);
  if(s.contains(2)) cout << "It contains 2!" << endl;
  else cout << "It doesn't contain 2!" << endl;
  s.print();
}
