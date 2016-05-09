#include "quadtree.hpp"

Quadtree::Quadtree() {
  root = NULL;
}
  
Quadtree::~Quadtree() {
  root->deleteNode();
}

int Quadtree::compare(const Point& p, QuadtreeNode* node) {
  // Node already exists
  if(p == node->point) return 0;
  if(p.x >= node->point.x) {
    if(p.y >= node->point.y) return NE;
    else return SE;
  }
  else {
    if(p.y >= node->point.y) return NW;
    else return SW;    
  }
}

bool Quadtree::insert(const Point& p) {
  if(not root) {
    root = new QuadtreeNode;
    root->point = p;
    root->quadrants = vector<QuadtreeNode*>(4,NULL);
    return true;
  }
  int direction = compare(p,root);
  if(not direction) return false;
  QuadtreeNode** quadrant = &(root->quadrants[direction-1]);
  while(*quadrant) {
    direction = compare(p,*quadrant);
    quadrant = &((*quadrant)->quadrants[direction-1]);
  }
  *quadrant = new QuadtreeNode;
  (*quadrant)->point = p;
  (*quadrant)->quadrants = vector<QuadtreeNode*>(4,NULL);
  return true;
}

bool Quadtree::insert(QuadtreeNode* node) {
  if(node) {
    if(not insert(node->point)) return false;
    for(int i = 0; i < 4; i++) {
      insert(node->quadrants[i]);
    }
    return true;
  }
  return false;
}

void Quadtree::search(const Point& p){
	cout << "point (" << p.x << "," << p.y << ")";
	if (search_rec(root, p)) cout <<" is in the tree";
	else cout <<" is not in the tree";
	cout << endl;
}

bool Quadtree::search_rec(QuadtreeNode* node, const Point& p){
	int direction = compare(p,node);
	if(not direction) return true;
	QuadtreeNode* child = node->quadrants[direction-1];
	if (not child) return false;
	return search_rec(child,p);
}

QuadtreeNode* Quadtree::searchNode(QuadtreeNode* node, const Point& p){
	int direction = compare(p,node);
	if(not direction) return node;
	if (not node->quadrants[direction-1]) return NULL;
	return searchNode(node->quadrants[direction-1],p);
}

void Quadtree::regionsearch(int L, int R, int B, int T){
	points = vector<Point>(0);
	LP = L;
	RP = R;
	BP = B;
	TP = T;
	regionsearch_rec(root,L,R,B,T);
	cout << "region [" << L << "," << R << "," << B << "," << T << "] has";
	if (points.size() > 0){
		 for (int i = 0; i < points.size(); ++i)  cout << " (" << points[i].x << "," << points[i].y << ")";
		 cout << endl;
	}
	else {
		cout << " no points inside" << endl;
	}
}

bool Quadtree::in_region(const Point& p){
	return p.x >= LP and p.x <= RP and p.y >= BP and p.y <= TP;
}


bool Quadtree::rectangle_overlaps_region(int L, int R, int B, int T){
	return L <= RP and R >= LP and B <= TP and T >= BP;
}

void Quadtree::regionsearch_rec(QuadtreeNode* node, int L, int R, int B, int T){
	if (in_region(node->point)) points.push_back(node->point);
	int x = node->point.x;
	int y = node->point.y;
	if (node->quadrants[0] and rectangle_overlaps_region(x,R,y,T)) regionsearch_rec(node->quadrants[0],x,R,y,T);
	if (node->quadrants[1] and rectangle_overlaps_region(L,x,y,T)) regionsearch_rec(node->quadrants[1],L,x,y,T);
	if (node->quadrants[2] and rectangle_overlaps_region(L,x,B,y)) regionsearch_rec(node->quadrants[2],L,x,B,y);
	if (node->quadrants[3] and rectangle_overlaps_region(x,R,B,y)) regionsearch_rec(node->quadrants[3],x,R,B,y);	
}

int Quadtree::conjugate(int n) {
  return ((n+1) % 4) + 1;
}

QuadtreeNode* Quadtree::findCandidate(QuadtreeNode* node, int quadrant) {
  if(node) {
    int conj = conjugate(quadrant);
    QuadtreeNode* candidate = findCandidate(node->quadrants[conj-1],conj);
    if(not candidate) return node;
    return candidate;
  }
  return node;
}

bool Quadtree::isInCrossSection(QuadtreeNode* quadrantRoot, QuadtreeNode* deletedNode, QuadtreeNode* candidate) const {
  if(deletedNode->point.x < candidate->point.x) {
    if(quadrantRoot->point.x >= deletedNode->point.x and quadrantRoot->point.x <= candidate->point.x) 
      return true;
  }
  else {
    if(quadrantRoot->point.x >= candidate->point.x and quadrantRoot->point.x <= deletedNode->point.x) 
      return true;
  }
  if(deletedNode->point.y < candidate->point.y) {
    if(quadrantRoot->point.y >= deletedNode->point.y and quadrantRoot->point.y <= candidate->point.y)
      return true;
  }
  else {
    if(quadrantRoot->point.y >= candidate->point.y and quadrantRoot->point.y <= deletedNode->point.y)
      return true;
  }
  return false;
}

// [Le paso el quadtree nuevo que queda porque me parecia mas follon editar el quadtree que llevasemos hasta ahora!]
void Quadtree::ADJ(QuadtreeNode* quadrantRoot, int quadrantId, QuadtreeNode* deletedNode, QuadtreeNode* candidate, Quadtree& newQuadtree) {
  if(quadrantRoot) {
    if(isInCrossSection(quadrantRoot,deletedNode,candidate)) {
      newQuadtree.insert(quadrantRoot);
    }
    else {
      if(quadrantId == NE or quadrantId == NW) {
	QuadtreeNode* se = quadrantRoot->quadrants[SE-1];
	QuadtreeNode* sw = quadrantRoot->quadrants[SW-1];
	quadrantRoot->quadrants[SE-1] = NULL;
	quadrantRoot->quadrants[SW-1] = NULL;
	newQuadtree.insert(quadrantRoot);
	ADJ(sw,SW,deletedNode,candidate,newQuadtree);
	ADJ(se,SE,deletedNode,candidate,newQuadtree);
      }
      else if(quadrantId == SE or quadrantId == SW) {
	QuadtreeNode* ne = quadrantRoot->quadrants[NE-1];
	QuadtreeNode* nw = quadrantRoot->quadrants[NW-1];
	quadrantRoot->quadrants[NE-1] = NULL;
	quadrantRoot->quadrants[NW-1] = NULL;
	newQuadtree.insert(quadrantRoot);
	ADJ(nw,NW,deletedNode,candidate,newQuadtree);
	ADJ(ne,NE,deletedNode,candidate,newQuadtree);
      }
    }
  }
}

void Quadtree::remove(const Point& p) {
  QuadtreeNode* node = searchNode(root,p);
  if(node) {
    vector<QuadtreeNode*> candidates(4);
    cout << "-- Candidates -- " << endl;
    for(int i = 0; i < 4; i++) {
      candidates[i] = findCandidate(node->quadrants[i],i+1);
      if(not candidates[i]) {
	candidates[i] = new QuadtreeNode;
	candidates[i]->point = Point(INT_MAX,INT_MAX);
      }
      cout << candidates[i]->point << endl;
    }
    cout << "-------" << endl;
    vector<int> finalCandidates;
    // Choose as candidate the one that is closer to each of its bordering axes than any other candidate which is on the
    // same side of these axes.
    if(candidates[NE-1]->point.x < candidates[SE-1]->point.x and candidates[NE-1]->point.y < candidates[NW-1]->point.y) 
      finalCandidates.push_back(NE-1);
    else if(candidates[NW-1]->point.x > candidates[SW-1]->point.x and candidates[NW-1]->point.y < candidates[NE-1]->point.y) 
      finalCandidates.push_back(NW-1);
    else if(candidates[SW-1]->point.x > candidates[NW-1]->point.x and candidates[SW-1]->point.y > candidates[SE-1]->point.y) 
      finalCandidates.push_back(SW-1);
    else if(candidates[SE-1]->point.x < candidates[NE-1]->point.x and candidates[SE-1]->point.y > candidates[SW-1]->point.y) 
      finalCandidates.push_back(SE-1);
    int candidateQuadrant = 0;
    // If none of the candidates or several of them meet the property, we choose the candidate with minimum L1 metric value.
    // The L1 metric measures the minimum sum of horizontal and vertical displacements from the node to be deleted. 
    if(finalCandidates.empty() or finalCandidates.size() > 1) {
      int minSum = INT_MAX; 
      for(int i = 0; i < 4; i++) {
	int sum = abs(candidates[i]->point.x - node->point.x) + abs(candidates[i]->point.y - node->point.y);
	if(sum < minSum) {
	  minSum = sum;
	  candidateQuadrant = i;
	}
      }
    }
    else candidateQuadrant = finalCandidates[0];
    Quadtree newQuadtree;
    newQuadtree.insert(candidates[candidateQuadrant]);
    newQuadtree.insert(node->quadrants[(candidateQuadrant+2)%4]);
    ADJ(node->quadrants[(candidateQuadrant+1)%4],(candidateQuadrant+2)%4,node,candidates[candidateQuadrant],newQuadtree);
    ADJ(node->quadrants[(candidateQuadrant+3)%4],(candidateQuadrant+3)%4,node,candidates[candidateQuadrant],newQuadtree);
    // [De momento asi pero porque me parecia mas follon editar el quadtree que llevasemos construido hasta ahora]
    root = new QuadtreeNode;
    root->clone(newQuadtree.getRoot());
  }
}

QuadtreeNode* Quadtree::getRoot() const {
  return root;
}

void Quadtree::showQuadtree() const {
  if(root) {
    root->showQuadtreeNode("","");
  }
}
