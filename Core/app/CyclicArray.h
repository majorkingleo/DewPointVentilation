/*
 * @author Copyright (c) 2023 Martin Oberzalek
 * From: https://stackoverflow.com/a/56334722
 */

#ifndef SRC_CYCLICARRAY_H_
#define SRC_CYCLICARRAY_H_

#include <cstddef>
#include <array>
#include <queue>
#include <stdexcept>

template <typename T, size_t N = 2>
class CyclicArray {
 public:
  typedef typename std::array<T, N>::value_type value_type;
  typedef typename std::array<T, N>::reference reference;
  typedef typename std::array<T, N>::const_reference const_reference;
  typedef typename std::array<T, N>::size_type size_type;

  static constexpr size_t npos = N;

  class iterator
  {
  private:
	  CyclicArray<T,N> * data = nullptr;
	  size_t pos = 0;

  public:
	  iterator( CyclicArray<T,N> * data_, size_t pos_ )
	  : data( data_ ),
		pos( pos_ )
	  {}

	  bool operator!=( const iterator & other_it ) {
		  if( data == other_it.data ) {
			  if( pos == other_it.pos ) {
				  return false;
			  }
		  }

		  return true;
	  }

	  T & operator*() {
		  return data->at(pos);
	  }

	  const T & operator*() const {
		  return data->at(pos);
	  }

	  iterator & operator++() {
		  pos++;
		  return *this;
	  }

	  iterator & operator--() {
		  pos--;
		  return *this;
	  }
  };

 public:

  ~CyclicArray() {
  	while (size()) {
  	  pop_front();
  	}
  }

  void push_back(const T& v) {
    if (size_ + 1 > N) {
      throw;
    }
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
  	if (size_ < 1) {
  	  throw;
  	}

  	front().~T();
  	++front_;
  	--size_;
  	if (front_ >= N)
  	  front_ = 0;
  }

  const_reference front() const {
  	return array_[front_];
  }

  reference front() {
  	return array_[front_];
  }

  size_type size() const {
    return size_;
  }

  bool empty() const {
	 return size() == 0;
  }

  const_reference operator[]( size_t n ) const {
  	return array_[(front_+n)%N];
  }

  reference operator[]( size_t n ) {
  	return array_[(front_+n)%N];
  }

  reference at( size_t pos ) {
	  if (pos >= size()) {
		  throw std::out_of_range( std::to_string(pos) + " is out of range" );
	  }
	  return array_[(front_+pos)%N];
  }

  const_reference at( size_t pos ) const {
	  if (pos >= size()) {
		  throw std::out_of_range();
	  }
	  return array_[(front_+pos)%N];
  }

  iterator begin() {
	  return iterator(this,0);
  }

  iterator end() {
	  return iterator(this,npos);
  }

 private:
  size_type front_ = 0;
  size_type size_ = 0;
  std::array<T, N> array_;
};


#endif /* SRC_CYCLICARRAY_H_ */
