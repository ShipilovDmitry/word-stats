#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <set>

int main(int argc, char *argv[]) {
  auto const cmdArguments = getCmdAgruments(argc, argv);
  if (!cmdArguments) {
    return -1;
  }

  auto wordFrequency = getStatsTable(cmdArguments->inputFile);
  struct MyCmp {
    bool operator()(std::pair<size_t, std::string> const &lhs,
                    std::pair<size_t, std::string> const &rhs) const {
      return lhs.first > rhs.first || lhs.second < rhs.second;
    }
  };

  // TODO rename 
  std::set<std::pair<size_t, std::string>, MyCmp> convenient;
  for (auto &word : wordFrequency) {
    convenient.emplace(word.second, std::move(word.first));
  }

  std::ofstream fout(cmdArguments->outputFile);
  if (!fout.is_open()) {
    std::cerr << "Error opening output file: " << cmdArguments->outputFile
              << '\n';
    return -1;
  }

  for (const auto &pair : convenient) {
    fout << pair.first << ' ' << pair.second
         << '\n'; // Don't flush IO buffer intentionally
  }

  return 0;
}
