#pragma once

#include "types.hpp"
#include <optional>

std::optional<CmdArguments> getCmdAgruments(int argc, char *argv[]);
