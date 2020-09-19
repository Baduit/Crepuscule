#pragma once

#include <vector>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>
#include <crepuscule/TokenPrinter.hpp>

namespace crepuscule
{

Expression tokenize(std::string_view input, const Config& config);

} // namespace crepuscule