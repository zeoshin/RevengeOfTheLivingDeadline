#ifndef POORMAN_PRIORITY_QUEUE_H
#define POORMAN_PRIORITY_QUEUE_H

#include "eecs281priority_queue.h"

//A specialized version of the 'heap' ADT that is implemented with an
//underlying unordered array-based container that is linearly searches
//for the min every time it is requested.
template<typename TYPE, typename COMP = std::less<TYPE>>
class poorman_priority_queue : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  poorman_priority_queue(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  poorman_priority_queue(COMP comp = COMP());

  //Description: Does nothing for this implementation, as items can never be
  //             'out of order'. This function is implemented for you.
  //Runtime: O(1)
  virtual void make_heap() {}

  //Description: Add a new element to the heap.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(n)
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(n)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap. This has been
  //             implemented for you.
  //Runtime: O(1)
  virtual size_type size() const { return data.size(); }

  //Description: Return true if the heap is empty. This has been implemented
  //             for you.
  //Runtime: O(1)
  virtual bool empty() const { return data.empty(); }
private:
  //Note: This vector *must* be used your heap implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
  void swap(TYPE &v1, TYPE &v2);
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
poorman_priority_queue<TYPE, COMP>::poorman_priority_queue(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
  InputIterator it;
  this->compare = comp;
  for(it = start; it != end; it++){
    data.push_back((*it));
  }
}

template<typename TYPE, typename COMP>
poorman_priority_queue<TYPE, COMP>::poorman_priority_queue(COMP comp) {
  //Your code.
  this->compare = comp;
}

template<typename TYPE, typename COMP>
void poorman_priority_queue<TYPE, COMP>::swap(TYPE &v1, TYPE &v2){
  TYPE temp = v1;
  v1 = v2;
  v2 = temp;
}

template<typename TYPE, typename COMP>
void poorman_priority_queue<TYPE, COMP>::push(const TYPE& val) {
  //Your code.
  data.push_back(val);
}

template<typename TYPE, typename COMP>
void poorman_priority_queue<TYPE, COMP>::pop() {
  //Your code.
  if(!empty()){
	unsigned i = 0;
    unsigned result_idx = i;
    for(; i < size(); i ++){
	  if(!this->compare(data[i], data[result_idx])) 
		result_idx = i;
	}
	swap(data[result_idx], data[size() - 1]);
	data.pop_back();
  }
}

template<typename TYPE, typename COMP>
const TYPE& poorman_priority_queue<TYPE, COMP>::top() const {
  //Your code.
  if(!empty()){
	unsigned i = 0;
    unsigned result_idx = i;
    for(; i < size(); i ++){
	  if(!this->compare(data[i], data[result_idx])) 
		result_idx = i;
	}
    return data[result_idx];
  }
  return TYPE(); //So that this sample implementation compiles.
}

#endif //POORMAN_PRIORITY_QUEUE_H
