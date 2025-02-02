#pragma once

#include <set>
#include <string>
#include <unordered_map>

namespace word_stats {

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
      return lhs.second < rhs.second; // Sort lexicographically
    }

    return lhs.first > rhs.first; // Sort by count
  }
};

using PreparedOutput =
    std::set<std::pair<size_t, std::string>, CountDescLexicAscComparator>;
} // namespace word_stats
