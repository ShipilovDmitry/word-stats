#pragma once

#include "types.hpp"
#include <chrono>

namespace word_stats {

class ExecuteDuration {
public:
  ExecuteDuration();
  ~ExecuteDuration();

private:
  std::chrono::high_resolution_clock::time_point m_start;
};

void doLogic(int argc, char *argv[]);

CmdArguments getCmdAgruments(int argc, char *argv[]);

StatsTable getStatsTable(std::string const &input);

PreparedOutput prepareOutput(StatsTable table);

void writeOutputToFile(PreparedOutput output, std::string const &outputFile);
} // namespace word_stats
