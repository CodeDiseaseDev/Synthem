#pragma once

#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>

#ifndef VecType
#define VecType float
#endif

class SafeVector {
public:
  SafeVector(const SafeVector& orig) : vec(orig.vec), mut(), cond() {}
  ~SafeVector() {}
  SafeVector() : vec(), mut(), cond() {}

  SafeVector(SafeVector&& orig)
    : vec{ std::move(orig.vec) },
    mut{},
    cond{}
  {}

  SafeVector(std::vector<VecType> vec)
    : vec{ std::move(vec) },
    mut{},
    cond{}
  {}

  VecType& operator[](const int index);
  std::vector<VecType>& toVector();

  void insert(VecType in, const int index);
  void push_back(VecType in);
  std::vector<VecType>::iterator begin();
  std::vector<VecType>::iterator end();
  void clear();
  size_t size();

private:
  std::vector<VecType> vec;
  std::mutex mut;
  std::condition_variable cond;
};