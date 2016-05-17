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

int Quadtree::conjugate(int n) const {
  return ((n+1) % 4) + 1;
}

QuadtreeNode* Quadtree::findCandidate(QuadtreeNode* node, int quadrant) const{
  if(node) {
    int conj = conjugate(quadrant);
    QuadtreeNode* candidate = findCandidate(node->quadrants[conj-1],conj);
    if(not candidate) return node;
    return candidate;
  }
  return node;
}

// We assume that the candidate for replacement of the node to remove has already been computed
// before calling this function.
bool Quadtree::isInCrossSection(QuadtreeNode* quadrantRoot) const {
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

bool Quadtree::ADJ(QuadtreeNode* quadrantRoot, int quadrantAdjId, int quadrantCandId) {
  if(quadrantRoot) {
    if(isInCrossSection(quadrantRoot)) {
      insert(quadrantRoot);
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
      bool newInsertion = ADJ(q,quadrantAdjId,quadrantCandId);
      if(not newInsertion) quadrantRoot->quadrants[q1-1] = q;
      q = quadrantRoot->quadrants[q2-1];
      quadrantRoot->quadrants[q2-1] = NULL;
      ADJ(q,quadrantAdjId,quadrantCandId);
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
	candidates[i] = findCandidate(node->quadrants[i],i+1);
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
	  int sum = abs(candidates[i]->point.x - node->point.x) + abs(candidates[i]->point.y - node->point.y);
	  if(sum < minSum) {
	    minSum = sum;
	    candidateQuadrant = i;
	  }
	}
      }
      else candidateQuadrant = finalCandidates[0];
      // [OJO: He puesto esto para no perder la info de los nodos porque en el Newroot se necesita, 
      // pero ponlo como quieras!]
      nodeRemoval = new QuadtreeNode;
      *nodeRemoval = *node;
      rootRemoval = new QuadtreeNode;
      *rootRemoval = *candidates[candidateQuadrant];
      // The conjugate quadrant remains the same.
      candidates[candidateQuadrant]->quadrants[conjugate(candidateQuadrant+1)-1] = node->quadrants[conjugate(candidateQuadrant+1)-1];
      // [OJO!: Se que esto sobrescribe lo que hubiese en los cuadrantes del candidato, pero de momento lo dejo asi porque 
      // se tendra que acabar poniendo lo que hubiese en el nodo a borrar en el nodo nuevo! De todas maneras, la info 
      // de los cuadrantes del nodo nuevo esta en rootRemoval.]
      candidates[candidateQuadrant]->quadrants[(candidateQuadrant+1)%4] = node->quadrants[(candidateQuadrant+1)%4];
      candidates[candidateQuadrant]->quadrants[(candidateQuadrant+3)%4] = node->quadrants[(candidateQuadrant+3)%4];
      *node = *candidates[candidateQuadrant];
      // Adjacent quadrants to the one containing the candidate new root.
      ADJ(node->quadrants[(candidateQuadrant+1)%4],(candidateQuadrant+1)%4+1,candidateQuadrant+1);
      ADJ(node->quadrants[(candidateQuadrant+3)%4],(candidateQuadrant+3)%4+1,candidateQuadrant+1);
    }
  }
}

void Quadtree::showQuadtree() const {
  if(root) {
    root->showQuadtreeNode("","");
  }
}
