#pragma once

#include <vector>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>

namespace crepuscule
{

std::vector<Token> tokenize(std::string_view input, const Config& config);

} // namespace crepuscule