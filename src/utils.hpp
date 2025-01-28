#pragma once

#include "types.hpp"

CmdArguments getCmdAgruments(int argc, char *argv[]);

void doLogic(int argc, char *argv[]);

StatsTable getStatsTable(std::string const &input);

PreparedOutput prepareOutput(StatsTable table);

void writeOutputToFile(PreparedOutput output, std::string const &outputFile);
