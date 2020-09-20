#pragma once

#include <vector>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>

namespace crepuscule
{

class Tokenizer
{
	public:
		Tokenizer(Config conf);

		Expression operator()(std::string_view input);

	private:
		Config _config;
};

} // namespace crepuscule