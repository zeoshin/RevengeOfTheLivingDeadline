#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "eecs281priority_queue.h"
#include <vector>

//A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class binary_heap : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  binary_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  binary_heap(COMP comp = COMP());

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap.
  //Runtime: O(log(n))
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const
  {
    return data.size();
  }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const
  {
    return (size() == 0);
  }
private:
  //Note: This vector *must* be used your heap implementation.
  std::vector<TYPE> data;
private:
  //***Add any additional member functions or data you require here.
  //unsigned size_type QueueCounter;
  void swap(TYPE &v1, TYPE &v2);
  void fixup(int k, COMP comp);
  void fixdown(int N, int k, COMP comp);
};
template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::swap(TYPE &v1, TYPE &v2){
  TYPE temp = v1;
  v1 = v2;
  v2 = temp;
}
template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::fixup(int k, COMP comp){
  while (k > 0 && comp(data[(k - 1)/2], data[k]))
  {
    swap(data[k], data[(k - 1)/2]); 
    k = (k - 1)/2;
  }
}
template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::fixdown(int N, int k, COMP comp){
  while (2*k + 1 <= N) 
  { 
    int j = 2*k + 1; 
 	if (j < N && comp(data[j], data[j+1])) ++j; 
 	if (!comp(data[k], data[j])) break; 
 	swap(data[k], data[j]); k = j; 
  }
}

template<typename TYPE, typename COMP>
template<typename InputIterator>
binary_heap<TYPE, COMP>::binary_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //My code.
  InputIterator it;
  this->compare = comp;
  for(it = start; it != end; it++){
    data.push_back((*it));
  }
  make_heap();
}

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp) {
  //My code.
  this->compare = comp;
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::make_heap() {
  for(int i = (size() - 1)/ 2; i >= 0; i--)
   fixdown(size() - 1, i, this->compare);
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::push(const TYPE& val) {
  //My code.
  data.push_back(val);
  fixup(size() - 1, this->compare);
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::pop() {
  //My code.
  swap(data[0], data[size() - 1]);
  if(!empty()) data.pop_back();
  fixdown(size() - 1, 0, this->compare);
}

template<typename TYPE, typename COMP>
const TYPE& binary_heap<TYPE, COMP>::top() const {
  if(!empty()) return data[0];
  return TYPE(); //This line present only so that this provided file compiles.
}

#endif //BINARY_HEAP_H

