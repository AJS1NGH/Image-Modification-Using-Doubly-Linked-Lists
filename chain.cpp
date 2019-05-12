#include "chain.h"
#include "chain_given.cpp"

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain(){
	clear();
}

/**
 * Inserts a new node at the end of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block & ndata){
	Node *n = new Node(ndata);
	width_ = ndata.width();
	height_ = ndata.height();
	n->prev = head_->prev;
	head_->prev->next = n;
	head_->prev = n;
	n->next = head_;
	length_ = length_ + 1;
}

/**
 * Modifies the Chain by moving the subchain of len Nodes,
 * starting at position startPos, dist positions toward the
 * end of the chain.  If startPos + len - 1 + dist > length
 * then dist = length - startPos - len + 1 (to prevent gaps
 * in the new chain).
 * The subchain occupies positions (startPos + dist) through
 * (startPos + dist + len - 1) of the resulting chain.
 * The order of subchain nodes is not changed in the move.
 * You may assume that: 1 <= startPos <= length - len + 1,
 * 0 <= dist <= length, and 0 <= len <= length. 
 */
void Chain::moveBack(int startPos, int len, int dist){
	if (dist != 0 && len!= 0) {
		int checkInputs = startPos + len - 1 + dist;
		if (checkInputs > length_) {
			dist = length_ - startPos - len + 1;
		}
		if (len == 1 && dist == 1) {
			Node* n = walk(head_, startPos);//n = ptr to node len
			n->next->prev = n->prev;
			n->prev->next = n->next;
			n->prev = n->next;
			n->next =n->next->next;
			n->next->prev = n;
			n->prev->next = n;
			return;
		}
		vector <int> lenIdx(2);
		vector <int> distIdx(2);
		if (len == 1) {
			lenIdx.resize(1);
			lenIdx[0] = startPos;
		}
		if (dist == 1) {
			distIdx.resize(1);
			distIdx[0] = startPos + len;//check this
		}
		if (lenIdx.size() == 2) {
			lenIdx[0] = startPos;
			lenIdx[1] = startPos + len -1;
		}
		if (distIdx.size() == 2) {
			distIdx[0] = startPos + len;
			distIdx[1] = startPos + len + dist - 1;
		}
		vector <Node*> lenPtrs(lenIdx.size());
		vector <Node*> distPtrs(distIdx.size());
		for (int i = 0; i < (int)lenIdx.size(); i++) {
			lenPtrs[i] = walk(head_,lenIdx[i]);
		}
		for (int i = 0; i < (int)distIdx.size(); i++) {
			distPtrs[i] = walk(head_, distIdx[i]);
		}
		int len_dptr = distPtrs.size();
		int len_lptr = lenPtrs.size();
		lenPtrs[0]->prev->next = distPtrs[0];
		distPtrs[len_dptr-1]->next->prev = lenPtrs[len_lptr-1];
		lenPtrs[len_lptr-1]->next = distPtrs[len_dptr-1]->next;
		distPtrs[0]->prev = lenPtrs[0]->prev;
		lenPtrs[0]->prev = distPtrs[len_dptr-1];
		distPtrs[len_dptr-1]->next = lenPtrs[0];
	}
}

/**
 * Rolls the current Chain by k nodes: reorders the current list
 * so that the first k nodes are the (n-k+1)th, (n-k+2)th, ... , nth
 * nodes of the original list followed by the 1st, 2nd, ..., (n-k)th
 * nodes of the original list where n is the length.
 */
void Chain::roll(int k){
	int s = size();
	if (s == 0 || s == 1 || s == k || k == 0) {return;}
	Node * last = walk(head_, s);
	Node * first = walk(head_, s-k+1);
	head_->next->prev = last;
	last->next = head_->next;
	head_->next = first;
	head_->prev = first->prev;
	first->prev->next = head_;
	first->prev = head_;
}

/**
 * Modifies the current chain by reversing the sequence
 * of nodes from pos1 up to and including pos2. You may
 * assume that pos1 and pos2 exist in the given chain,
 * and pos1 <= pos2.
 * The positions are 1-based.
 */
void Chain::reverseSub(int pos1, int pos2){
	int p = pos2-pos1;
	if (p==0) {return;}
	for (int i = 0; i < p; i++) {
		Node * p1 = walk(head_,pos1+i);
		Node * p2 = walk(p1, p-i);//reduce recursive walk amount
		p2->prev->next = p2->next;
		p2->next->prev = p2->prev;
		p2->prev = p1->prev;
		p2->prev->next = p2;
		p1->prev = p2;
		p2->next = p1;
	}
}

/*
* Modifies both the current chain and the "other" chain by removing
* nodes from the other chain and adding them between the nodes
* of the current chain -- one "other" node between two current nodes --
* until one of the two chains is exhausted.  Then the rest of the
* nodes in the non-empty chain follow.
* The length of the resulting chain should be the sum of the lengths
* of current and other. The other chain should have only
* the head_ sentinel at the end of the operation.
* The weave fails and the original
* chains should be unchanged if block sizes are different.
* In that case, the result of the function should be:
* cout << "Block sizes differ." << endl;
*/
void Chain::weave(Chain & other) { // leaves other empty.
  int o_size = other.size();
  int t_size = size();
  if (o_size == 0 && t_size == 0) {return;}
  Node *oHead = other.head();
  Node *oFirst = oHead->next;
  if ((oFirst->data.height() != head_->next->data.height()) ||
  	  (oFirst->data.width() != head_->next->data.width())) {
  	   cout << "Block sizes differ." << endl; return;}

  //combine both lists together as they are originally.
  //put all of other chains nodes infront of our last node. 
  //Maintaing Order of other's nodes.
  Node *tLast = head_->prev;
  Node *oHeadTemp = oFirst->prev;
  Node *oLast = oHeadTemp->prev;
  oHead->prev = oHeadTemp;
  tLast->next->prev = oLast;
  oLast->next = tLast->next;
  oFirst->prev = tLast;
  tLast->next = oFirst;
  oHead->next = oHeadTemp;
  length_ = o_size+t_size;
  other.length_ = 0;

  int loop_size;
  int move_size;

  if (t_size < o_size) { loop_size = t_size-1; move_size = t_size;}
  else if (t_size > o_size) {loop_size = t_size-1; move_size = t_size-1;}
  else {loop_size = o_size-1; move_size=o_size-1;}
  //sort
  for (int i = 0; i < loop_size; i++) {
  	moveBack(2,1,move_size+i);
  }
}


/**
 * Destroys all dynamically allocated memory associated with the
 * current Chain class except for the sentinel head_. Sets length_
 * to zero.  After clear() the chain represents an empty chain.
 */
void Chain::clear() {
	Node * curr = head_->next;
	while (curr!= head_) {
		Node * n = curr->next;
		curr->next = NULL;
		curr->prev = NULL;
		delete curr;
		curr = n;
	}
	head_->next = head_;
	head_->prev = head_;
	length_ = 0;
	width_ = 0;
	height_ = 0;
	delete head_;
	head_ = NULL;
}

/**
 * Makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const& other) {
	  Node * curr = other.head()->next;
	  while (size() != other.size()) {
	  	insertBack(curr->data);
	  	curr = curr->next;
	  }
}
/**
 * helper function
 */
typename Chain::Node * Chain::head() const {
	return head_;
}
