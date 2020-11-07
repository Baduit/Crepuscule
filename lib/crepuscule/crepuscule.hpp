#pragma once

#include <vector>
#include <optional>
#include <tuple>

#include <crepuscule/Config.hpp>
#include <crepuscule/Token.hpp>
#include <crepuscule/ProcessingState.hpp>

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
		Result operator()(std::string&& input) const;

	private:
		void tokenize(Result& result) const;

		enum class IterationState
		{
			CONTINUE_ITERATION,
			NEXT_ITERATION
		};

		void prepare_config();

		IterationState handle_end_comment(ProcessingState& state) const;
		IterationState handle_end_string(ProcessingState& state) const;
		IterationState handle_solo_and_begin_delimiters(ProcessingState& state) const;
		IterationState handle_end_expression(ProcessingState& state) const;
		void handle_ordinary_character(ProcessingState& state) const;

		void handle_line(ProcessingState& state, Result& result) const;
		void handle_last_line(ProcessingState& state, Result& result) const;

		void consume_word(ProcessingState& state) const;
		auto get_it_delimiters(std::string_view current_view) const;

		
		template <typename ItComment, typename ItString, typename ItSubexpression, typename ItOpe, typename ItDelim>
		auto consume_delimiters(ProcessingState& state, ItComment comment, ItString string, ItSubexpression subexpression, ItOpe ope, ItDelim delimiter) const
		{
			auto comment_size = (comment) ? (*comment)->begin.size() : 0;
			auto string_size = (string) ? (*string)->begin.size() : 0;
			auto subexpression_size = (subexpression) ? (*subexpression)->begin.size() : 0;
			auto ope_size = (ope) ? (*ope)->size() : 0;
			auto delimiter_size = (delimiter) ? (*delimiter)->size() : 0;

			auto max_size = std::max({comment_size, string_size, subexpression_size, ope_size, delimiter_size});
			if (max_size == comment_size)
			{
				state.begin_comment(**comment);
			}
			else if (max_size == string_size)
			{
				state.begin_string(**string);
			}
			else if (max_size == subexpression_size)
			{
				state.begin_expression(**subexpression);
			}
			else if (max_size == ope_size)
			{
				state.emplace_token<Operator>(**ope, state.get_current_line_number());
			}
			// Nothing to do if it is just a delimiter
			return max_size;
		}

		template<typename Method, typename... FollowingMethods> 
		void process(ProcessingState& state, Method method) const
		{
			(this->*method)(state);
		}


		template<typename Method, typename... FollowingMethods> 
		void process(ProcessingState& state, Method method, FollowingMethods&&... following_methods) const
		{
			IterationState iteration_state = (this->*method)(state);
			if (iteration_state == IterationState::CONTINUE_ITERATION)
				process(state, std::forward<FollowingMethods>(following_methods)...);
		}

	private:
		Config _config;
};

} // namespace crepuscule