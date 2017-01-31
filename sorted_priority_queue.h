#ifndef SORTED_PRIORITY_QUEUE_H
#define SORTED_PRIORITY_QUEUE_H

#include "eecs281priority_queue.h"
#include <algorithm>

//A specialized version of the 'heap' ADT that is implemented with an
//underlying sorted array-based container.
//Note: The most extreme element should be found at the end of the
//'data' container, such that traversing the iterators yields the elements in
//sorted order.
template<typename TYPE, typename COMP = std::less<TYPE>>
class sorted_priority_queue : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n log n) where n is number of elements in range.
  template<typename InputIterator>
  sorted_priority_queue(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  sorted_priority_queue(COMP comp = COMP());

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n log n)
  virtual void make_heap();

  //Description: Add a new element to the heap.
  //Runtime: O(n)
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(1)
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
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
sorted_priority_queue<TYPE, COMP>::sorted_priority_queue(
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
  make_heap();
}

template<typename TYPE, typename COMP>
sorted_priority_queue<TYPE, COMP>::sorted_priority_queue(COMP comp) {
  //Your code.
  this->compare = comp;
}

template<typename TYPE, typename COMP>
void sorted_priority_queue<TYPE, COMP>::swap(TYPE &v1, TYPE &v2){
  TYPE temp = v1;
  v1 = v2;
  v2 = temp;
}

template<typename TYPE, typename COMP>
void sorted_priority_queue<TYPE, COMP>::make_heap() {
  //Your code.
  std::sort(data.begin(), data.end(), this->compare);
}

template<typename TYPE, typename COMP>
void sorted_priority_queue<TYPE, COMP>::push(const TYPE& val) {
  //Your code.
  data.push_back(val);
  //std::sort(data.begin(), data.end(), this->compare);
  unsigned i = size() - 1;
  for(; i >= 1; --i){
	if(this->compare(data[i], data[i - 1]))
	  swap(data[i], data[i - 1]);
  }
}

template<typename TYPE, typename COMP>
void sorted_priority_queue<TYPE, COMP>::pop() {
  //Your code.
  data.pop_back();
}

template<typename TYPE, typename COMP>
const TYPE& sorted_priority_queue<TYPE, COMP>::top() const {
  //Your code.
  if(!empty()) return data[size() - 1];
  return TYPE(); //So that this sample implementation compiles.
}

#endif //SORTED_PRIORITY_QUEUE_H
