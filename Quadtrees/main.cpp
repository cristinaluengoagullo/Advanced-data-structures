#include "quadtree.hpp"
#include <string>

Point getpoint(string input){
		int i = 0;
		int a,b;
		string coord1,coord2;
		while (i < input.size() and input[i] != '(') ++i;
		++i;
		while (i < input.size() and input[i] != ','){ coord1.push_back(input[i]); ++i; }
		if (coord1 == "*") a = INT_MAX;
		else a = stoi(coord1);
		++i;
		while (i < input.size() and input[i] != ')'){ coord2.push_back(input[i]); ++i; }
		if (coord2 == "*") b = INT_MAX;
		else b = stoi(coord2);
		Point p(a,b);
		return p;
}

int main() {
	Quadtree root;
	string input;
	cout << endl << "---------- Choose an option ----------" << endl;
    cout << endl << "show: prints the quadtree." << endl;
	cout << "example: loads the example quadtree." << endl;
	cout << "clean: resets the whole tree." << endl;
	cout << "insert (a,b): inserts point (a,b) in the tree." << endl;
	cout << "remove (a,b): removes point (a,b) from the tree if it's inside." << endl;
	cout << "search (a,b): checks if point (a,b) is in in the tree. For partial match put * in a or b."<< endl;
	cout << "region [L, R, B, T]: prints all nodes inside the region."<< endl;
	cout << "exit: closes the app."<< endl;
	cout << endl << "--> ";
	while(cin>>input){
		if(input.find("info") != string::npos){
            cout << endl << "show: prints the quadtree." << endl;
			cout << "example: loads the example quadtree." << endl;
			cout << "clean: resets the whole tree." << endl;
			cout << "insert (a,b): inserts point (a,b) in the tree." << endl;
			cout << "remove (a,b): removes point (a,b) from the tree if it's inside." << endl;
			cout << "search (a,b): checks if point (a,b) is in in the tree. For partial match put * in a or b."<< endl;
			cout << "region [L, R, B, T]: prints all nodes inside the region."<< endl;
			cout << "exit: closes the app."<< endl;
		}
        else if(input.find("show") != string::npos) root.showQuadtree();
        else if(input.find("clean") != string::npos){
			root.~Quadtree();
		}
        else if(input.find("example") != string::npos){
		  root.~Quadtree();
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
			
			cout << "Example loaded" << endl;
		}
        else if(input.find("insert") != string::npos){
			cin >> input;
			Point p = getpoint(input);
			if(p.x == INT_MAX or p.y == INT_MAX) cout << "invalid point to insert" << endl;
			else {
				root.insert(p);
				cout << "point inserted" << endl;
			}
		}
        else if(input.find("remove") != string::npos){
			cin >> input;
			Point p = getpoint(input);
			root.remove(p);
			if(p.x == INT_MAX or p.y == INT_MAX) cout << "invalid point to remove" << endl;
			else {
				root.remove(p);
				cout << "point removed" << endl;
			}
			
		}
        else if(input.find("search") != string::npos){
			cin >> input;
			Point p = getpoint(input);
			if(p.x == INT_MAX or p.y == INT_MAX){
				cout << "------ Partial match query -------" << endl;
				vector<Point> res = root.partialMatch(p);
				for(int i = 0; i < res.size(); i++)
				cout << res[i] << endl;
				cout << "-----------------------------------" << endl;
			}
			else root.search(p);
		}
        else if(input.find("region") != string::npos){
			cin >> input;
			int i = 0;
			int l,r,b,t;
			string coord1,coord2,coord3,coord4;
			while (i < input.size() and input[i] != '[') ++i;
			++i;
			while (i < input.size() and input[i] != ','){ coord1.push_back(input[i]); ++i; }
			l = stoi(coord1);
			++i;
			while (i < input.size() and input[i] != ','){ coord2.push_back(input[i]); ++i; }
			r = stoi(coord2);
			++i;
			while (i < input.size() and input[i] != ','){ coord3.push_back(input[i]); ++i; }
			b = stoi(coord3);
			++i;
			while (i < input.size() and input[i] != ']'){ coord4.push_back(input[i]); ++i; }
			t = stoi(coord4);
			root.regionsearch(l,r,b,t);
		}
        else if(input.find("exit") != string::npos) break;
		else cout << "Wrong option!" << endl;
		cout << "--> ";
	}
}
