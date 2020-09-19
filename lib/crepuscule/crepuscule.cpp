#include <optional>
#include <stdexcept>

#include <crepuscule/crepuscule.hpp>
#include <crepuscule/helpers.hpp>

namespace crepuscule
{

Expression tokenize(std::string_view input, const Config& config)
{
	Expression main_expression;
	std::vector<Expression*> expression_stack { &main_expression };

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
				expression_stack.back()->value.emplace_back(Comment(it_token_begin, it_input, std::move(*current_comment_delimiter)));
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
				expression_stack.back()->value.emplace_back(std::move(*current_string));
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
			auto comment = helpers::to_optional_it(helpers::find_delimiter_begin(config.comment_delimiters, current_view), config.comment_delimiters.end());
			auto string = helpers::to_optional_it(helpers::find_delimiter_begin(config.string_delimiters, current_view), config.string_delimiters.end());
			auto subexpression = helpers::to_optional_it(helpers::find_delimiter_begin(config.subexpression_delimiters, current_view), config.subexpression_delimiters.end());
			auto ope = helpers::to_optional_it(helpers::find_at_start(config.operators, current_view), config.operators.end());
			auto delimiter = helpers::to_optional_it(helpers::find_at_start(config.delimiters, current_view), config.delimiters.end());
			
			
			if (comment || string || subexpression || ope || delimiter)
			{
				std::string_view word = std::string_view(it_token_begin, it_input);
				if (!word.empty())
				{
					bool word_used = false;

					auto keyword = std::find(config.keywords.begin(), config.keywords.end(), word);
					if (keyword != config.keywords.end())
					{
						expression_stack.back()->value.emplace_back(Keyword {*keyword});
						word_used = true;
					}

					if (!word_used && config.integer_reader)
					{
						auto integer = config.integer_reader(word);
						if (integer)
						{
							expression_stack.back()->value.emplace_back(Integer{ *integer });
							word_used = true;
						}
					}

					if (!word_used && config.number_reader)
					{
						auto number = config.number_reader(word);
						if (number)
						{
							expression_stack.back()->value.emplace_back(Number{ *number });
							word_used = true;
						}
					}

					if (!word_used)
						expression_stack.back()->value.emplace_back(Word{ std::string(word) });
				}

				auto comment_size = (comment) ? (*comment)->begin.size() : 0;
				auto string_size = (string) ? (*string)->begin.size() : 0;
				auto subexpression_size = (subexpression) ? (*subexpression)->begin.size() : 0;
				auto ope_size = (ope) ? (*ope)->size() : 0;
				auto delimiter_size = (delimiter) ? (*delimiter)->size() : 0;

				auto max_size = std::max({comment_size, string_size, subexpression_size, ope_size, delimiter_size});
				if (max_size == comment_size)
				{
					current_comment_delimiter.emplace(**comment);
				}
				else if (max_size == string_size)
				{
					current_string.emplace(String{ "", **string });
				}
				else if (max_size == subexpression_size)
				{
					expression_stack.back()->value.emplace_back(Expression{ {}, **subexpression });
					auto* new_expression = std::get_if<Expression>(&(expression_stack.back()->value.back()));
					expression_stack.push_back(new_expression);
				}
				else if (max_size == ope_size)
				{
					expression_stack.back()->value.emplace_back(Operator{ **ope });
				}
				// Nothing to do if it is just a delimiter

				it_input += max_size;
				it_token_begin = it_input;
			}
			else
			{
				if (expression_stack.back()->delimiter &&
					current_view.starts_with(expression_stack.back()->delimiter->end))
				{
					it_input += expression_stack.back()->delimiter->end.size();
					it_token_begin = it_input;

					expression_stack.pop_back();
				}
				else
				{
					++it_input;
				}
			}
		}
	}

	return main_expression;
}

} // namespace crepuscule