#include "utils.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace word_stats {

ExecuteDuration::ExecuteDuration() {
  m_start = std::chrono::high_resolution_clock::now();
}

ExecuteDuration::~ExecuteDuration() {
  auto finish = std::chrono::high_resolution_clock::now();
  std::cout << "Duration: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
                                                                     m_start)
                   .count()
            << " ms" << std::endl;
}

CmdArguments getCmdAgruments(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " path/to/input.txt path/to/output.txt"
              << std::endl;
    throw std::logic_error{"Check args!"};
  }

  return {argv[1], argv[2]};
};

bool isLowercaseChar(char c) { return c >= 'a' && c <= 'z'; }
bool isUppercaseChar(char c) { return c >= 'A' && c <= 'Z'; }

StatsTable fillStatsFromFile(std::ifstream &fin) {
  StatsTable stats;
  std::string word;

  constexpr size_t kChunkSize = 1024; // We can tune chunks size for efficiency
  std::array<char, kChunkSize> buffer;

  // Do not read char by char for performance
  while (!fin.eof()) {
    fin.read(buffer.data(), buffer.size());
    size_t const justRead = fin.gcount();

    for (size_t i = 0; i < justRead; ++i) {
      auto const c = buffer[i];
      if (isLowercaseChar(c)) {
        word.push_back(c);
        continue;
      }

      if (isUppercaseChar(c)) {
        word.push_back(std::tolower(c));
        continue;
      }

      if (word.empty()) {
        // Skip not latin symbols
        continue;
      }

      ++stats[word];
      word.clear();
    }
  }

  return stats;
}

StatsTable fillStatsFromFileSlow(std::ifstream &fin) {
  StatsTable stats;
  std::string word;

  while (!fin.eof()) {
    char const c = fin.get();
    if (c >= 'a' && c <= 'z') {
      word.push_back(c);
      continue;
    }

    if (c >= 'A' && c <= 'Z') {
      word.push_back(std::tolower(c));
      continue;
    }

    if (word.empty()) {
      continue;
    }

    ++stats[word];
    word.clear();
  }

  return stats;
}

StatsTable getStatsTable(std::string const &input) {
  std::ifstream fin(input);
  if (!fin.is_open()) {
    std::cerr << "Incorrect input file!" << input << std::endl;
    throw std::logic_error{"Incorrect input file!"};
  }
  return fillStatsFromFile(fin);
}

PreparedOutput prepareOutput(StatsTable wordFrequency) {
  PreparedOutput output;
  for (auto &word : wordFrequency) { // non const because of std::move
    output.emplace(word.second, std::move(word.first));
  }
  return output;
}

void writeOutputToFile(PreparedOutput output, std::string const &outputFile) {
  std::ofstream fout(outputFile);
  if (!fout.is_open()) {
    std::cerr << "Error opening output file: " << outputFile << '\n';
    throw std::logic_error{"Error opening output file"};
  }

  for (auto const &pair : output) {
    fout << pair.first << ' ' << pair.second
         << '\n'; // Don't flush IO buffer intentionally
  }
}

void doLogic(int argc, char *argv[]) {
  auto const cmdArgs = getCmdAgruments(argc, argv);
  auto output = prepareOutput(getStatsTable(cmdArgs.inputFile));
  writeOutputToFile(std::move(output), cmdArgs.outputFile);
}
} // namespace word_stats
