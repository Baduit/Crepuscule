#pragma once

#include <vector>
#include <optional>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>

namespace crepuscule
{

// The expression and lines are invalidated if text goes out of scope
struct Result
{
	std::string text;
	std::vector<std::string_view> lines;
	Expression expression;
};

class Tokenizer
{
	public:
		Tokenizer(const Config& conf);
		Tokenizer(Config&& conf);

		Result operator()(std::string_view input) const;

	private:
		static constexpr std::string_view endline_delimiter = "\n";

		void prepare_config();

	private:
		Config _config;
};

} // namespace crepuscule