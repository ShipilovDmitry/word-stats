#pragma once

#include "types.hpp"

namespace word_stats {

void doLogic(int argc, char *argv[]);

CmdArguments getCmdAgruments(int argc, char *argv[]);

StatsTable getStatsTable(std::string const &input);

PreparedOutput prepareOutput(StatsTable table);

void writeOutputToFile(PreparedOutput output, std::string const &outputFile);
} // namespace word_stats
