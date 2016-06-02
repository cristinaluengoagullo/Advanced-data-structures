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
  return 0;
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
    if(not direction) return false;
    quadrant = &((*quadrant)->quadrants[direction-1]);
  }
  *quadrant = new QuadtreeNode;
  (*quadrant)->point = p;
  (*quadrant)->quadrants = vector<QuadtreeNode*>(4,NULL);
  return true;
}

bool Quadtree::insert(QuadtreeNode* node) {
  if(node) {
    int direction = compare(node->point,root);
    if(not direction) return false;
    QuadtreeNode** quadrant = &(root->quadrants[direction-1]);
    while(*quadrant) {
      direction = compare(node->point,*quadrant);
      if(not direction) return false;
      quadrant = &((*quadrant)->quadrants[direction-1]);
    }
    *quadrant = new QuadtreeNode;
    *(*quadrant) = *node;
    return true;
  }
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

Point Quadtree::nearestNeighbor(const Point& p) const {
}

vector<Point> Quadtree::partialMatchRec(QuadtreeNode* node, const Point& p) const {
  vector<Point> matches;
  if(node) {
    int quad1, quad2;
    if(p.x < INT_MAX) {
      if(node->point.x == p.x) {
	matches.push_back(node->point);
	quad1 = NE; quad2 = SE;
      }
      else if(node->point.x < p.x) {
	quad1 = NE; quad2 = SE;
      }
      else {
	quad1 = NW; quad2 = SW;
      }
    }
    else if(p.y < INT_MAX) {
      if(node->point.y == p.y) {
	matches.push_back(node->point);
	quad1 = NE; quad2 = NW;
      }
      else if(node->point.y > p.y) {
	quad1 = SW; quad2 = SE;
      }
      else {
	quad1 = NE; quad2 = NW;
      }
    }
    vector<Point> res = partialMatchRec(node->quadrants[quad1-1],p);
    matches.insert(matches.end(),res.begin(),res.end());
    res = partialMatchRec(node->quadrants[quad2-1],p);
    matches.insert(matches.end(),res.begin(),res.end());
  }
  return matches;
}

vector<Point> Quadtree::partialMatch(const Point& p) const {
  if(root) 
    return partialMatchRec(root,p);
  return vector<Point>();
}

int Quadtree::conjugate(int n) const {
  return ((n+1) % 4) + 1;
}

QuadtreeNode* Quadtree::findCandidate(QuadtreeNode* node, int quadrant) const{
  if(node) {
    QuadtreeNode* candidate = findCandidate(node->quadrants[quadrant-1],quadrant);
    if(not candidate) return node;
    return candidate;
  }
  return node;
}

// We assume that the candidate for replacement of the node to remove has already been computed
// before calling this function.
bool Quadtree::isInCrossSection(QuadtreeNode* quadrantRoot, QuadtreeNode* rootRemoval, QuadtreeNode* nodeRemoval) const {
  if(nodeRemoval->point.x < rootRemoval->point.x) {
    if(quadrantRoot->point.x >= nodeRemoval->point.x and quadrantRoot->point.x <= rootRemoval->point.x) 
      return true;
  }
  else {
    if(quadrantRoot->point.x >= rootRemoval->point.x and quadrantRoot->point.x <= nodeRemoval->point.x) 
      return true;
  }
  if(nodeRemoval->point.y < rootRemoval->point.y) {
    if(quadrantRoot->point.y >= nodeRemoval->point.y and quadrantRoot->point.y <= rootRemoval->point.y)
      return true;
  }
  else {
    if(quadrantRoot->point.y >= rootRemoval->point.y and quadrantRoot->point.y <= nodeRemoval->point.y)
      return true;
  }
  return false;
}

void Quadtree::NewRoot(QuadtreeNode* quadrantRoot, QuadtreeNode* rootRemoval, QuadtreeNode* nodeRemoval, int direction){
	if(quadrantRoot){
		int q1,q2;
		q1 = (direction+1) % 4;
		q2 = (direction-1) % 4;
		QuadtreeNode* father = quadrantRoot;
		while(quadrantRoot->quadrants[direction]){
			QuadtreeNode* q = quadrantRoot->quadrants[q1];
			quadrantRoot->quadrants[q1] = NULL;
			bool newInsertion = ADJ(q,rootRemoval,nodeRemoval,q1+1,conjugate(direction+1));
			if(not newInsertion) quadrantRoot->quadrants[q1] = q;
			q = quadrantRoot->quadrants[q2];
			quadrantRoot->quadrants[q2] = NULL;
			newInsertion = ADJ(q,rootRemoval,nodeRemoval,q2+1,conjugate(direction+1));
			if(not newInsertion) quadrantRoot->quadrants[q2] = q;
			father = quadrantRoot;
			quadrantRoot = quadrantRoot->quadrants[direction];	
		}
		if(quadrantRoot->quadrants[q1]){
			reinsertions.push_back(quadrantRoot->quadrants[q1]);
			quadrantRoot->quadrants[q1] = NULL;	
		}
		if(quadrantRoot->quadrants[q2]){
			reinsertions.push_back(quadrantRoot->quadrants[q2]);
			quadrantRoot->quadrants[q2] = NULL;
		}
		father->quadrants[direction] = quadrantRoot->quadrants[(direction+2)%4];
		quadrantRoot->quadrants[(direction+2)%4] = NULL;
	}	
}

bool Quadtree::ADJ(QuadtreeNode* quadrantRoot, QuadtreeNode* rootRemoval, QuadtreeNode* nodeRemoval, int quadrantAdjId, int quadrantCandId) {
  if(quadrantRoot) {
    if(isInCrossSection(quadrantRoot,rootRemoval,nodeRemoval)) {
      reinsertions.push_back(quadrantRoot);
      return true;
    }
    else {
      int q1, q2;
      switch(quadrantAdjId) {
      case NE:
	if(quadrantCandId == NW) { q1 = SW; q2 = SE; }
	else if(quadrantCandId == SE) { q1 = NW; q2 = SW; } break;
      case NW:
	if(quadrantCandId == NE) { q1 = SW; q2 = SE; }
	else if(quadrantCandId == SW) { q1 = NE; q2 = SE; } break;
      case SW:
	if(quadrantCandId == NW) { q1 = NE; q2 = SE; }
	else if(quadrantCandId == SE) { q1 = NW; q2 = NE; } break;
      case SE:
	if(quadrantCandId == NE) { q1 = NW; q2 = SW; }
	else if(quadrantCandId == SW) { q1 = NW; q2 = NE; } break;
      }
      QuadtreeNode* q = quadrantRoot->quadrants[q1-1];
      quadrantRoot->quadrants[q1-1] = NULL;
      bool newInsertion = ADJ(q,rootRemoval,nodeRemoval,quadrantAdjId,quadrantCandId);
      if(not newInsertion) quadrantRoot->quadrants[q1-1] = q;
      q = quadrantRoot->quadrants[q2-1];
      quadrantRoot->quadrants[q2-1] = NULL;
      newInsertion = ADJ(q,rootRemoval,nodeRemoval,quadrantAdjId,quadrantCandId);
      if(not newInsertion) quadrantRoot->quadrants[q2-1] = q;
    }
  }
  return false;
}

vector<int> Quadtree::getCandidates(QuadtreeNode* node, const vector<QuadtreeNode*>& candidates) const {
  vector<int> finalCandidates;
  // Choose as candidate the one that is closer to each of its bordering axes than any other candidate which is on the
  // same side of these axes.
  if(abs(candidates[NE-1]->point.x - node->point.x) < abs(candidates[SE-1]->point.x - node->point.x)) {
    if(abs(candidates[NE-1]->point.y - node->point.y) < abs(candidates[NW-1]->point.y - node->point.y))
      finalCandidates.push_back(NE-1);
  }
  if(abs(candidates[NW-1]->point.x - node->point.x) < abs(candidates[SW-1]->point.x - node->point.x)) {
    if(abs(candidates[NW-1]->point.y - node->point.y) < abs(candidates[NE-1]->point.y - node->point.y))
      finalCandidates.push_back(NW-1);
  }
  if(abs(candidates[SW-1]->point.x - node->point.x) < abs(candidates[NW-1]->point.x - node->point.x)) {
    if(abs(candidates[SW-1]->point.y - node->point.y) < abs(candidates[SE-1]->point.y - node->point.y))
      finalCandidates.push_back(SW-1);
  }
  if(abs(candidates[SE-1]->point.x - node->point.x) < abs(candidates[NE-1]->point.x - node->point.x)) {
    if(abs(candidates[SE-1]->point.y - node->point.y) < abs(candidates[SW-1]->point.y - node->point.y))
      finalCandidates.push_back(SE-1);
  }
  return finalCandidates;
}

void Quadtree::removeTerminalNode(const Point& p) {
  if(root) {
    int direction = compare(p,root);
    if(not direction) root = NULL;
    QuadtreeNode** quadrant = &(root->quadrants[direction-1]);
    while(*quadrant) {
      direction = compare(p,*quadrant);
      if(not direction) {
	*quadrant = NULL;
	break;
      }
      quadrant = &((*quadrant)->quadrants[direction-1]);
    }
  }
}

void Quadtree::remove(const Point& p) {
  QuadtreeNode* node = searchNode(root,p);
  if(node) {
    reinsertions = vector<QuadtreeNode*>();
    bool terminalNode = true;
    for(int i = 0; i < 4; i++) {
      if(node->quadrants[i]) {
	terminalNode = false;
	break;
      }
    }
    // If it is a terminal node, remove directly just by cutting links with its root.
    if(terminalNode) removeTerminalNode(node->point);
    else {
      vector<QuadtreeNode*> candidates(4);
      for(int i = 0; i < 4; i++) {
	candidates[i] = findCandidate(node->quadrants[i],conjugate(i+1));
	if(not candidates[i]) {
	  candidates[i] = new QuadtreeNode;
	  candidates[i]->point = Point(INT_MAX,INT_MAX);
	}
      }
      vector<int> finalCandidates = getCandidates(node,candidates);
      int candidateQuadrant = 0;
      // If none of the candidates or several of them meet the property, we choose the candidate with minimum L1 metric value.
      // The L1 metric measures the minimum sum of horizontal and vertical displacements from the node to be deleted. 
      if(finalCandidates.empty() or finalCandidates.size() > 1) {
	int minSum = INT_MAX; 
	for(int i = 0; i < 4; i++) {
	  int sum = INT_MAX;
	  if(candidates[i]->point.x < INT_MAX) 
	    sum = abs(candidates[i]->point.x - node->point.x) + abs(candidates[i]->point.y - node->point.y);
	  if(sum < minSum) {
	    minSum = sum;
	    candidateQuadrant = i;
	  }
	}
      }
      else candidateQuadrant = finalCandidates[0];
      // Adjacent quadrants to the one containing the candidate new root.
		
	QuadtreeNode* q = node->quadrants[(candidateQuadrant+1)%4];
	node->quadrants[(candidateQuadrant+1)%4] = NULL;
	bool newInsertion = ADJ(q,candidates[candidateQuadrant],node,(candidateQuadrant+1)%4+1,candidateQuadrant+1);
	if(not newInsertion) node->quadrants[(candidateQuadrant+1)%4] = q;
	q = node->quadrants[(candidateQuadrant+3)%4];
	node->quadrants[(candidateQuadrant+3)%4] = NULL;
	newInsertion =  ADJ(q,candidates[candidateQuadrant],node,(candidateQuadrant+3)%4+1,candidateQuadrant+1);
	if(not newInsertion) node->quadrants[(candidateQuadrant+3)%4] = q;
			
	NewRoot(node->quadrants[candidateQuadrant],candidates[candidateQuadrant],node,conjugate(candidateQuadrant+1)-1);
      
	node->point = candidates[candidateQuadrant]->point;
	
	cout << "Reinsertions: " << endl;
      for(int i = 0; i < reinsertions.size(); i++) {
	insert(reinsertions[i]);
	cout << reinsertions[i]->point << endl;
      }
      cout << "-----------" << endl;
    }
  }
}

void Quadtree::showQuadtree() const {
  if(root) {
    root->showQuadtreeNode("","");
  }
}
