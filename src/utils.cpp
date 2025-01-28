#include "utils.hpp"
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

StatsTable getStatsTable(std::string const &input) {
  std::ifstream fin(input);
  if (!fin.is_open()) {
    std::cerr << "Incorrect input file!" << input << std::endl;
    throw std::logic_error{"Incorrect input file!"};
  }

  StatsTable table;
  std::string word;

  // TODO read more intellectual, use chunks of 1KB
  while (!fin.eof()) {
    // Do we need check for fin.fail() or fin.bad()?

    // It is possible to use std::alpa()
    // but we don't want to cal `tolower` on lowe case letters
    char const c = fin.get();
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

  return table;
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
