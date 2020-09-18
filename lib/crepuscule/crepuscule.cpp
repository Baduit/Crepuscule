#include <optional>

#include <crepuscule/crepuscule.hpp>
#include <crepuscule/helpers.hpp>

namespace crepuscule
{

std::vector<Token> tokenize(std::string_view input, const Config& config)
{
	Expression main_expression;

	// Should never be nullptr, not a reference so it can be rebindable
	Expression* current_expression = &main_expression; 
	std::optional<StringDelimiter> string_delimiter;
	std::optional<CommentDelimiter> comment_delimiter;

	auto it_input = input.begin();
	auto it_token_begin = it_input;
	while (it_input != input.end())
	{
		std::string_view current_view = std::string_view(it_input, input.end());
		if (comment_delimiter)
		{
			if (current_view.starts_with(comment_delimiter->end))
			{
				current_expression->value.emplace_back(MultilineComment(it_token_begin, it_input, std::move(*comment_delimiter)));
				comment_delimiter.reset();
				it_token_begin = it_input + 1;
			}
			++it_input;
		}
		else if (string_delimiter)
		{
			++it_input;
		}
		else
		{
			++it_input;
		}
	}
	(void) config;
	return main_expression.value;
}

} // namespace crepuscule