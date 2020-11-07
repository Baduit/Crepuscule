#include <crepuscule/ProcessingState.hpp>
#include <crepuscule/endline.hpp>

namespace crepuscule
{

ProcessingState::ProcessingState(std::string_view input, Expression& main_expression)
{
	_expression_stack.push_back(&main_expression);
	_it_input = input.cbegin();
	update_token_begin();
	_it_end = input.cend();
	_it_line_begin = _it_input;
}	

ProcessingState::operator bool() const
{
	return _it_input != _it_end;
}

std::string_view ProcessingState::get_current_view() const
{
	// MSVC does not support this for now
	//return std::string_view(_it_input, _it_end);
	auto size = static_cast<std::size_t>(_it_end - _it_input);
	return std::string_view(&(*_it_input), size);
}

std::string_view ProcessingState::get_current_word() const
{
	// MSVC does not support this for now
	//return std::string_view(_it_token_begin, _it_input);
	auto size = static_cast<std::size_t>(_it_input - _it_token_begin);
	return std::string_view(&(*_it_token_begin), size);
}

void ProcessingState::update_token_begin()
{
	_it_token_begin = _it_input;
}

void ProcessingState::add_current_char_to_current_string()
{
	_current_string->value += *_it_input;
}

void ProcessingState::add_to_current_string(std::string_view str)
{
	_current_string->value += str;
}

void ProcessingState::begin_string(const StringDelimiter& delim)
{
	_current_string.emplace(delim);
}

void ProcessingState::begin_comment(const CommentDelimiter& delim)
{
	_current_comment_delimiter.emplace(delim);
}

void ProcessingState::begin_expression(const SubexpressionDelimiter& delim)
{
	emplace_token<Expression>(delim);
	auto* new_expression = std::get_if<Expression>(&(_expression_stack.back()->value.back()));
	_expression_stack.push_back(new_expression);
}

std::optional<std::string_view> ProcessingState::get_current_string_end_delimiter() const
{
	if (_current_string)
		return _current_string->delimiter.end;
	else
		return {};
}

std::optional<std::string_view> ProcessingState::get_current_comment_end_delimiter() const
{
	if (_current_comment_delimiter)
		return _current_comment_delimiter->end;
	else
		return {};
}

std::optional<std::string_view> ProcessingState::get_current_expression_end_delimiter() const
{
	if (_expression_stack.back()->delimiter)
		return _expression_stack.back()->delimiter->end;
	else
		return {};
	
}

void ProcessingState::close_current_expression()
{
	advance_input(_expression_stack.back()->delimiter->end.size());
	update_token_begin();
	_expression_stack.pop_back();
}
void ProcessingState::close_current_string()
{
	auto delimiter_size = get_current_string_end_delimiter()->size();
	emplace_token<String>(std::move(*_current_string));
	_current_string.reset();

	advance_input(delimiter_size);
	update_token_begin();
}
void ProcessingState::close_current_comment()
{
	auto delimiter_size = get_current_comment_end_delimiter()->size();
	emplace_token<Comment>(_it_token_begin, _it_input, std::move(*_current_comment_delimiter));
	_current_comment_delimiter.reset();
	advance_input(delimiter_size);
	update_token_begin();
}

std::string_view ProcessingState::retrieve_line()
{
	auto old_it_line_begin = _it_line_begin;
	_it_line_begin = _it_input + 1;

	// MSVC does not support this for now
	//return std::string_view(_it_token_begin, _it_input);
	auto size = static_cast<std::size_t>(_it_input - old_it_line_begin);
	return std::string_view(&(*old_it_line_begin), size + 1);
}

std::optional<std::string_view> ProcessingState::retrieve_last_line()
{
	auto size = static_cast<std::size_t>(_it_input - _it_line_begin);
	if (size != 0)
		return std::string_view(&(*_it_line_begin), size);
	else
		return {};
}

} // namespace crepuscule