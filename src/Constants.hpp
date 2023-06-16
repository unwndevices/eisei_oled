#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <vector>
#include <algorithm>
#include <string>

typedef std::pair<std::string, float> ConstantPair;

std::vector<ConstantPair> &getConstants();
#endif // CONSTANTS_HPP
