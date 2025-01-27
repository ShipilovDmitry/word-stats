#pragma once

#include "types.hpp"

#include <optional>
#include <map>

using StatsTable = std::map<std::string, size_t>;

std::optional<CmdArguments> getCmdAgruments(int argc, char *argv[]);

StatsTable getStatsTable(std::string const &input);
