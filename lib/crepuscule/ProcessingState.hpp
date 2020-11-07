#pragma once		

#include <concepts>

#include <crepuscule/Token.hpp>

namespace crepuscule
{

class ProcessingState
{
	public:
		ProcessingState(std::string_view input, Expression& main_expression);

		operator bool() const;

		std::string_view get_current_view() const;
		std::string_view get_current_word() const;

		template <std::integral I>
		void advance_input(I i)
		{
			_it_input += i;
		}

		void update_token_begin();

		void add_current_char_to_current_string();
		void add_to_current_string(std::string_view str);

		void begin_string(const StringDelimiter& delim);
		void begin_comment(const CommentDelimiter& delim);
		void begin_expression(const SubexpressionDelimiter& delim);

		template <typename T, typename... Args>
		void emplace_token(Args&&... args)
		{
			_expression_stack.back()->value.emplace_back(T(std::forward<Args>(args)...));
		}

		std::optional<std::string_view> get_current_string_end_delimiter() const;
		std::optional<std::string_view> get_current_comment_end_delimiter() const;
		std::optional<std::string_view> get_current_expression_end_delimiter() const;

		void close_current_expression();
		void close_current_string();
		void close_current_comment();

		std::string_view retrieve_line();
		std::optional<std::string_view> retrieve_last_line();

		std::size_t get_current_line_number() const;

	private:
		std::vector<Expression*> _expression_stack;
		std::optional<String> _current_string;
		std::optional<CommentDelimiter> _current_comment_delimiter;
		std::string_view::const_iterator _it_input;
		std::string_view::const_iterator _it_end;
		std::string_view::const_iterator _it_token_begin;
		std::string_view::const_iterator _it_line_begin;
		std::size_t _current_line = INVALID_LINE_NUMBER;
};

	
} // namespace crepuscule