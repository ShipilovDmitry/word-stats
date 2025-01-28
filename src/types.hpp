#pragma once

#include <set>
#include <string>
#include <unordered_map>

struct CmdArguments {
  std::string inputFile;
  std::string outputFile;
};

using StatsTable =
    std::unordered_map<std::string, size_t>; // Order doesn't matter

struct CountDescLexicAscComparator {
  bool operator()(std::pair<size_t, std::string> const &lhs,
                  std::pair<size_t, std::string> const &rhs) const {
    if (lhs.first == rhs.first) {
      return lhs.second < rhs.second;
    }

    return lhs.first > rhs.first;
  }
};

using PreparedOutput =
    std::set<std::pair<size_t, std::string>, CountDescLexicAscComparator>;
