#include "utils.hpp"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

CmdArguments getCmdAgruments(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " path/to/input.txt path/to/output.txt"
              << std::endl;
    throw std::logic_error{"Check args!"};
  }

  return CmdArguments{argv[1], argv[2]};
};

StatsTable readFromFile(std::ifstream &fin) {
  StatsTable table;
  std::string word;

  constexpr size_t kChunkSize = 1024; // We can tune chunks size for efficiency
  std::array<char, kChunkSize> chunks;

  // Do not read char by char for performance
  while (!fin.eof()) {
    fin.read(chunks.data(), chunks.size());
    auto const justRead = fin.gcount();

    for (size_t i = 0; i < justRead; ++i) {
      auto const c = chunks[i];
      if (c >= 'a' && c <= 'z') {
        word.push_back(c);
        continue;
      }

      if (c >= 'A' && c <= 'Z') {
        word.push_back(std::tolower(c));
        continue;
      }

      // We met not symbol
      // or reached end of file
      if (!word.empty()) {
        ++table[word];
        word.clear();
        continue;
      }
    }
  }

  return table;
}

StatsTable getStatsTable(std::string const &input) {
  std::ifstream fin(input);
  if (!fin.is_open()) {
    std::cerr << "Incorrect input file!" << input << std::endl;
    throw std::logic_error{"Incorrect input file!"};
  }
  return readFromFile(fin);
}

PreparedOutput prepareOutput(StatsTable wordFrequency) {
  std::set<std::pair<size_t, std::string>, CountDescLexicAscComparator> output;
  for (auto &word : wordFrequency) {
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
