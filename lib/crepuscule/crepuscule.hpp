#pragma once

#include <vector>
#include <optional>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>

namespace crepuscule
{

struct Result
{
	std::string text;
	std::vector<std::string_view> lines;
	Expression expression;
};

class Tokenizer
{
	public:
		Tokenizer(Config conf);

		Result operator()(std::string_view input);

	private:
		void prepare_config();

	private:
		Config _config;
};

} // namespace crepuscule