#pragma once

#include "types.hpp"

#include <optional>
#include <unordered_map>

using StatsTable =
    std::unordered_map<std::string, size_t>; // Order doesn't matter here

std::optional<CmdArguments> getCmdAgruments(int argc, char *argv[]);

StatsTable getStatsTable(std::string const &input);
