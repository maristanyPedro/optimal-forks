#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <cstddef> //For size_t.
#include <limits>

constexpr std::size_t INVALID = std::numeric_limits<std::size_t>::max();
constexpr int MAX_COST = std::numeric_limits<int>::max();

#endif