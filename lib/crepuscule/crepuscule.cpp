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
	std::optional<String> current_string;
	std::optional<CommentDelimiter> current_comment_delimiter;

	auto it_input = input.begin();
	auto it_token_begin = it_input;
	while (it_input != input.end())
	{
		std::string_view current_view = std::string_view(it_input, input.end());
		if (current_comment_delimiter)
		{
			if (current_view.starts_with(current_comment_delimiter->end))
			{
				auto delimiter_size = current_comment_delimiter->end.size();
				current_expression->value.emplace_back(MultilineComment(it_token_begin, it_input, std::move(*current_comment_delimiter)));
				current_comment_delimiter.reset();

				it_input += delimiter_size;
				it_token_begin = it_input;
			}
			else
			{
				++it_input;
			}
		}
		else if (current_string)
		{
			// TODO: This part can probably a bit optimized
			if (current_view.starts_with(current_string->delimiter.end))
			{
				auto delimiter_size = current_string->delimiter.end.size();
				current_expression->value.emplace_back(std::move(*current_string));
				current_string.reset();

				it_input += delimiter_size;
				it_token_begin = it_input;
			}
			else
			{
				if (auto custom_sequence = helpers::find_custom_sequence(config.custom_string_sequences, current_view);
					custom_sequence != config.custom_string_sequences.end())
				{
					current_string->value += custom_sequence->replacement;
					it_input += custom_sequence->sequence.size();
				}
				else
				{
					current_string->value += *it_input;
					++it_input;
				}
			}
		}
		else
		{
			++it_input;
		}
	}

	return main_expression.value;
}

} // namespace crepuscule