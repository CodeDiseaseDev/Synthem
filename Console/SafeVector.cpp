#include <string>
#include <utility>
#include "SafeVector.h"


void SafeVector::insert(VecType in, const int index)
{
  std::lock_guard<std::mutex> lock(mut);
  vec[index] = std::move(in);
  cond.notify_one();
}

void SafeVector::push_back(VecType in)
{
  std::lock_guard<std::mutex> lock(mut);
  vec.push_back(std::move(in));
  cond.notify_one();
}

void SafeVector::clear() {
  std::lock_guard<std::mutex> lock(mut);
  vec.clear();
  cond.notify_one();
}

size_t SafeVector::size() {
  return vec.size();
}

VecType& SafeVector::operator[](const int index)
{
  return vec[index];
}

std::vector<VecType>::iterator SafeVector::begin()
{
  return vec.begin();
}

std::vector<VecType>::iterator SafeVector::end()
{
  return vec.end();
}

std::vector<VecType>& SafeVector::toVector()
{
  return vec;
}