/*
 * @author Copyright (c) 20223 Martin Oberzalek
 * From: https://stackoverflow.com/a/56334722
 */

#ifndef SRC_CYCLICARRAY_H_
#define SRC_CYCLICARRAY_H_

#include <array>
#include <iostream>
#include <queue>

template <typename T, size_t N = 2>
class CyclicArray {
 public:
  typedef typename std::array<T, N>::value_type value_type;
  typedef typename std::array<T, N>::reference reference;
  typedef typename std::array<T, N>::const_reference const_reference;
  typedef typename std::array<T, N>::size_type size_type;

  ~CyclicArray() {
  	while (size())
  	  pop_front();
  }

  void push_back(const T& v) {
    if (size_ + 1 > N)
      throw;
    new (&array_[(front_ + size_) % N]) T(v);
    ++size_;
  }

  void push_back(const T& v, bool discard_front ) {
    if (size_ + 1 > N) {
    	pop_front();
    }
    new (&array_[(front_ + size_) % N]) T(v);
    ++size_;
  }

  void pop_front() {
  	if (size_ < 1)
  	  throw;
  	front().~T();
  	++front_;
  	--size_;
  	if (front_ >= N)
  	  front_ = 0;
  }

  const_reference front() const {
  	return *static_cast<const T*>(&array_[front_]);
  }

  reference front() {
  	return *reinterpret_cast<T*>(&array_[front_]);
  }

  size_type size() const {
    return size_;
  }

 private:
  size_type front_ = 0;
  size_type size_ = 0;
  std::array<char[sizeof(T)], N> array_;
};


#endif /* SRC_CYCLICARRAY_H_ */
