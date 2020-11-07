#include <optional>
#include <stdexcept>

#include <crepuscule/crepuscule.hpp>
#include <crepuscule/helpers.hpp>
#include <crepuscule/endline.hpp>

namespace crepuscule
{

Tokenizer::Tokenizer(const Config& conf):
	_config(conf)
{
	prepare_config();
}

Tokenizer::Tokenizer(Config&& conf):
	_config(std::move(conf))
{
	prepare_config();
}

Result Tokenizer::operator()(std::string_view input) const
{
	Result result;
	result.text = std::string(input);

	Expression& main_expression = result.expression;
	ProcessingState state(result.text, main_expression);

	while (state)
	{
		std::string_view current_view = state.get_current_view();

		if (current_view.starts_with(endline_delimiter))
			result.lines.push_back(state.retrieve_line());

		auto iteration_state = handle_end_comment(state);
		if (iteration_state == IterationState::CONTINUE_ITERATION)
			iteration_state = handle_end_string(state);
		if (iteration_state == IterationState::CONTINUE_ITERATION)	
			iteration_state = handle_solo_and_begin_delimiters(state);
		if (iteration_state == IterationState::CONTINUE_ITERATION)
			iteration_state = handle_end_expression(state);
		if (iteration_state == IterationState::CONTINUE_ITERATION)
			state.advance_input(1);
	}

	auto last_line = state.retrieve_last_line();
	if (last_line)
		result.lines.push_back(*last_line);

	return result;
}

void Tokenizer::prepare_config()
{
	auto sort_descending = 
		[](const auto& a, const auto& b)
		{
			return !(a < b);
		};

	std::sort(_config.string_delimiters.begin(), _config.string_delimiters.end(), sort_descending);
	std::sort(_config.operators.begin(), _config.operators.end(), sort_descending);
	std::sort(_config.delimiters.begin(), _config.delimiters.end(), sort_descending);
	std::sort(_config.keywords.begin(), _config.keywords.end(), sort_descending);
	std::sort(_config.custom_string_sequences.begin(), _config.custom_string_sequences.end(), sort_descending);
	std::sort(_config.subexpression_delimiters.begin(), _config.subexpression_delimiters.end(), sort_descending);
	std::sort(_config.comment_delimiters.begin(), _config.comment_delimiters.end(), sort_descending);
}

auto Tokenizer::get_it_delimiters(std::string_view current_view) const
{
	auto comment = helpers::to_optional_it(helpers::find_delimiter_begin(_config.comment_delimiters, current_view), _config.comment_delimiters.cend());
	auto string = helpers::to_optional_it(helpers::find_delimiter_begin(_config.string_delimiters, current_view), _config.string_delimiters.cend());
	auto subexpression = helpers::to_optional_it(helpers::find_delimiter_begin(_config.subexpression_delimiters, current_view), _config.subexpression_delimiters.cend());
	auto ope = helpers::to_optional_it(helpers::find_at_start(_config.operators, current_view), _config.operators.cend());
	auto delimiter = helpers::to_optional_it(helpers::find_at_start(_config.delimiters, current_view), _config.delimiters.cend());
	return std::tuple(comment, string, subexpression, ope, delimiter);
}

Tokenizer::IterationState Tokenizer::handle_end_comment(ProcessingState& state) const
{
	std::string_view current_view = state.get_current_view();
	if (auto comment_end_delimiter = state.get_current_comment_end_delimiter();
		comment_end_delimiter)
	{
		if (current_view.starts_with(*comment_end_delimiter))
			state.close_current_comment();
		else
			state.advance_input(1);
		return Tokenizer::IterationState::NEXT_ITERATION;
	}
	else
	{
		return IterationState::CONTINUE_ITERATION;
	}
}

Tokenizer::IterationState Tokenizer::handle_end_string(ProcessingState& state) const
{
	std::string_view current_view = state.get_current_view();
	if (auto string_end_delimiter = state.get_current_string_end_delimiter();
			string_end_delimiter)
	{
		// TODO: This part can probably a bit optimized
		if (current_view.starts_with(*string_end_delimiter))
		{
			state.close_current_string();
		}
		else
		{
			if (auto custom_sequence = helpers::find_custom_sequence(_config.custom_string_sequences, current_view);
				custom_sequence != _config.custom_string_sequences.end())
			{
				state.add_to_current_string(custom_sequence->replacement);
				state.advance_input(custom_sequence->sequence.size());
			}
			else
			{
				state.add_current_char_to_current_string();
				state.advance_input(1);
			}
		}
		return IterationState::NEXT_ITERATION;
	}
	else
	{
		return IterationState::CONTINUE_ITERATION;
	}
}

Tokenizer::IterationState Tokenizer::handle_solo_and_begin_delimiters(ProcessingState& state) const
{
	if (auto [comment, string, subexpression, ope, delimiter] = get_it_delimiters(state.get_current_view());
		comment || string || subexpression || ope || delimiter)
	{
		consume_word(state);
		auto max_size = consume_delimiters(state, comment, string, subexpression, ope, delimiter);
		state.advance_input(max_size);
		state.update_token_begin();
		return IterationState::NEXT_ITERATION;
	}
	else
	{
		return IterationState::CONTINUE_ITERATION;
	}
}

Tokenizer::IterationState Tokenizer::handle_end_expression(ProcessingState& state) const
{
	if (auto expression_delim = state.get_current_expression_end_delimiter();
		expression_delim && state.get_current_view().starts_with(*expression_delim))
	{
		state.close_current_expression();
		return IterationState::NEXT_ITERATION;
	}
	else
	{
		return IterationState::CONTINUE_ITERATION;
	}
}

void Tokenizer::consume_word(ProcessingState& state) const
{
	std::string_view word = state.get_current_word();
	if (!word.empty())
	{
		bool word_used = false;

		auto keyword = std::find(_config.keywords.begin(), _config.keywords.end(), word);
		if (keyword != _config.keywords.end())
		{
			state.emplace_token<Keyword>(*keyword);
			word_used = true;
		}

		if (!word_used && _config.integer_reader)
		{
			auto integer = _config.integer_reader(word);
			if (integer)
			{
				state.emplace_token<Integer>(*integer);
				word_used = true;
			}
		}

		if (!word_used && _config.number_reader)
		{
			auto number = _config.number_reader(word);
			if (number)
			{
				state.emplace_token<Number>(*number);
				word_used = true;
			}
		}

		if (!word_used)
			state.emplace_token<Word>(std::string(word));
	}
}

} // namespace crepuscule