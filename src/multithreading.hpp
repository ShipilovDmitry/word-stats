#pragma once
#include "types.hpp"
#include <string>

namespace word_stats {
StatsTable fillStatsFromFileAsync(std::string const &filename);
}
