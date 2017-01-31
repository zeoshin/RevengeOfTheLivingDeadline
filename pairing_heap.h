#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "eecs281priority_queue.h"


//A specialized version of the 'heap' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class pairing_heap : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  pairing_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  pairing_heap(COMP comp = COMP());
  
  //Description: Copy constructor.
  //Runtime: O(n)
  pairing_heap(const pairing_heap& other);
  
  //Description: Copy assignment operator.
  //Runtime: O(n)
  pairing_heap& operator=(const pairing_heap& rhs);
  
  //Description: Destructor
  //Runtime: O(n)
  ~pairing_heap();

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap. This has been provided for you,
  //             in that you should implement push functionality in the
  //             add_node function.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val) { add_node(val); }

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const { return Size; }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const { return Size == 0; }

  class Node {
    //This node class will allow external users to update the priority of
    //elements that are already inside the heap.
  public:
    //***Add any constructors you need here.
  public:
    //Description: Allows access to the element at that Node's position.
    //Runtime: O(1) - this has been provided for you.
    const TYPE& operator*() const { return elt; }

    //The following line allows you to access any private data members of this
    //Node class from within the pairing_heap class. (ie: myNode.elt is a legal
    //statement in pairing_heap's add_node() function).
    friend pairing_heap;
  private:
    TYPE elt;
  private:
    //***Add any additional member functions or data you require here.
	Node* Left;
	Node* Sibling;
	Node* Child;
  };

  //Description: Updates the priority of an element already in the heap by
  //             replacing the element refered to by the Node with new_value.
  //             Must maintain heap invariants.
  //
  //PRECONDITION: The new priority, given by 'new_value' must be more extreme
  //              (as defined by comp) than the old priority.
  //
  //Runtime: As discussed in reading material.
  void updateElt(Node* node, TYPE new_value);

  //Description: Add a new element to the heap. Returns a Node* corresponding
  //             to the newly added element.
  //Runtime: Amortized O(1)
  Node* add_node(const TYPE& val);

private:
  Node* Root;
  int Size;
  //***Add any additional member functions or data you require here.
  //***We recommend creating a 'meld' function (see the reading).
  Node* meld(Node* left, Node* right);
  void mergeHelper(Node* start);
  void rec_make(Node* start);
  Node* copy_const(Node* From);
};
template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::meld(Node *left, Node* right){
  if(right != nullptr && left != nullptr){
	if(this->compare(right->elt, left->elt)){
	  //printf("here\n");
	  left->Sibling = right->Sibling;
	  if(right->Sibling != nullptr) right->Sibling->Left = left;
	  right->Left = left;
	  right->Sibling = left->Child;
	  if(left->Child != nullptr)left->Child->Left = right;
	  left->Child = right;
	  //printf("left->Child->Left: %d\n", left->Child->Left->elt);
	  return left;
	}
	else{
	  right->Left = left->Left;
	  if(left->Left != nullptr && left->Left->Child == left)
		left->Left->Child = right;
	  if(left->Left != nullptr && left->Left->Child != left)
		left->Left->Sibling = right;
	  left->Sibling = right->Child;
	  left->Left = right;
	  if(right->Child != nullptr) right->Child->Left = left;
	  right->Child = left;
	  return right;
	}
  }
  else if(right != nullptr) return right;
  else return left;
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::mergeHelper(Node *start){
  if(start != nullptr){
	Node* first = new Node;
	Node* second = new Node;
	first = start;
	second = (start->Sibling == nullptr) ? 
			 nullptr: start->Sibling;
	while(first != nullptr && second != nullptr){
	  first = meld(first, second);
	  if(first->Sibling != nullptr){
		first = first->Sibling;
		second = first->Sibling;
	  }
	  else second = nullptr;
	}
	while(first != nullptr && first->Left != nullptr){
	  first = meld(first->Left, first);
	}
	if(first != nullptr){
	  if(Root != nullptr) delete Root;
	  Root = first;
	  Root->Left = nullptr;
	  Root->Sibling = nullptr;
	}
    }
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::rec_make(Node *start){
  if(start != NULL){
    if(start->Sibling != nullptr) rec_make(start->Sibling);
    if(start->Child != nullptr){
	  rec_make(start->Child);
  	  Node* curr = start->Child;
  	  Node* MAXNODE;
  	  TYPE MAXELT;
	  MAXNODE = start;
	  MAXELT = start->elt;
  	  while (curr != nullptr){
  	    if(this->compare(MAXELT, curr->elt)){
  	      MAXNODE = curr;
	      MAXELT = curr->elt;
  	    }
  	    curr = curr->Sibling;
  	  }
  	  MAXNODE->elt = start->elt;
  	  start->elt = MAXELT;
    }
  }
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::copy_const(Node* From){
  if(From == nullptr){
	return nullptr;
  }
  else{
	Node* newnode = new Node;
	newnode->elt = From->elt;
	newnode->Child = copy_const(From->Child);
	newnode->Sibling = copy_const(From->Sibling);
	if(newnode->Child !=nullptr) newnode->Child->Left = newnode;
	if(newnode->Sibling != nullptr) newnode->Sibling->Left = newnode;
	return newnode;
  }
}

template<typename TYPE, typename COMP>
template<typename InputIterator>
pairing_heap<TYPE, COMP>::pairing_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
  Size = 0;
  InputIterator it;
  this->compare = comp;
  Root = nullptr;
  for(it = start; it != end; it++){
    push((*it));
  }
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(COMP comp) {
  //Your code.
  Size = 0;
  Root = nullptr;
  this->compare = comp;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(const pairing_heap& other) {
  Root = nullptr;
  *this = other;
  //Your code.
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>&
pairing_heap<TYPE, COMP>::operator=(const pairing_heap& rhs) {
  //Your code.
  if(this != &rhs){
	Root = nullptr;
	Size = rhs.size();
	Root = copy_const(rhs.Root);
  }
  return *this;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::~pairing_heap() {
  while(Size != 0) pop();
  Root = nullptr;
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::make_heap() {
  //Your code.
  rec_make(Root);
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::pop() {
  if(Size > 0){
	if(Root->Child == nullptr) {delete Root; Root = nullptr;}
	else{
	  //Root->Child->Left = nullptr;
	  Node* temp = new Node;
	  temp = Root->Child;
	  temp->Left = nullptr;
	  if(Root != nullptr){
		delete Root;
		Root = nullptr;
	  }
	  mergeHelper(temp);
	}
	Size--;
  }
}

template<typename TYPE, typename COMP>
const TYPE& pairing_heap<TYPE, COMP>::top() const {
  //Your code.
  return Root->elt;
  return TYPE(); //This line present only so that this provided file compiles.
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::add_node(const TYPE& val) {
  //Your code.
  if(empty()){
	Node* n = new Node;
	n->Sibling = nullptr;
	n->Left = nullptr;
	n->Child = nullptr;
	n->elt = val;
    Size++;
	Root = n;
	return n;
  }
  else{
	Node* n = new Node;
	n->Sibling = nullptr;
	n->Left = Root;
	Root->Sibling = n;
	n->Child = nullptr;
	n->elt = val;
	Root = meld(Root, n);
    Size++;
	return n;
  }
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::updateElt(Node* node, TYPE new_value) {
  //Your code.
  if(this->compare(Root->elt, new_value)){
	node->elt = new_value;
	if(node->Left != nullptr){
	  if(node->Left->Child == node)
	    node->Left->Child = node->Sibling;
	  else 
	    node->Left->Sibling = node->Sibling;
	}
	if(node->Sibling != nullptr) 
	  node->Sibling->Left = node->Left;
	node->Left = nullptr;
	node->Sibling = nullptr;
	Root = meld(node, Root);
  }
}

#endif //PAIRING_HEAP_H

