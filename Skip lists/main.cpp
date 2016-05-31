#include "Set.hpp"
#include <math.h>

const int NUMBER_LISTS = 100;
const double GAMMA = 0.5772156649;

int main(int argc, char **argv) {
  for(int k = 2000; k <= 20000; k += 1000) {
    cout << "++++++++ n = " << k << " ++++++++" << endl;
    for(double l = 0.1; l <= 0.9; l += 0.1) {
      cout << "--- q =  " << l << " ---" << endl;
      double q = l;
      double Q = 1/q;
      double L = log(Q);
      int expected_total_cost = Q*k*(log(k)/log(Q))+k*((Q/L)*(GAMMA-1)+(1/L)-(Q/2));
      int expected_memory_usage = (k/(1-q))+(log(k)/log(Q));
      cout << expected_total_cost << endl;
      cout << expected_memory_usage << endl;
      int cost = 0;
      int memory = 0;
      // Insertion of elements in a list.
      for(int i = 0; i < NUMBER_LISTS; i++) {
	Set<int> s(q,k);
	for(int j = 1; j <= k; j++) {
	  s.insert(j);
	}
	cost += s.totalSearchCost(1,k);
	memory += s.numberPointers();
      }
      cout << (double)cost/NUMBER_LISTS << endl;
      cout << (double)memory/NUMBER_LISTS << endl;
      cout << "----------" << endl;
    }
    cout << "+++++++++++++++++++" << endl;
  }
}

