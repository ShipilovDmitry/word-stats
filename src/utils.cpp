#include "utils.hpp"
#include "multithreading.hpp"
#include "types.hpp"
#include <array>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
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

      if (std::isalpha(c)) {
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

StatsTable getStatsTable(std::string const &input) {
  std::ifstream fin(input);
  if (!fin.is_open()) {
    std::cerr << "Incorrect input file!" << input << std::endl;
    throw std::logic_error{"Incorrect input file!"};
  }

  return fillStatsFromFileAsync(input);
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
  ExecuteDuration d;
  auto const cmdArgs = getCmdAgruments(argc, argv);
  auto output = prepareOutput(getStatsTable(cmdArgs.inputFile));
  writeOutputToFile(std::move(output), cmdArgs.outputFile);
}
} // namespace word_stats
