#pragma once

#include <string>

/// ANSI escape codes for partial terminal highlighting (not full-line color).
namespace ansi {

inline const std::string reset = "\x1b[0m";
inline const std::string red = "\x1b[31m";
inline const std::string green = "\x1b[32m";
inline const std::string yellow = "\x1b[33m";

}  // namespace ansi
