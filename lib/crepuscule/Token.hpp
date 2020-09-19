#pragma once

#include <variant>
#include <optional>
#include <compare>

#include <crepuscule/Config.hpp>

namespace crepuscule
{

struct Keyword;
struct Word;
struct Integer;
struct Number;
struct String;
struct Comment;
struct Comment;
struct Expression;
struct Operator;

using Token = std::variant<Keyword, Word, Integer, Number, String, Operator, Comment, Expression>;

/*
	TODO: instead of making a deep copy of the delimiters I could:
	use a shared_ptr and always use a pointer to the same instance
	=> easy to use, hard to use the wrong way and probably faster (benchmark needed)
*/


struct Keyword
{
	friend bool operator==(const Keyword&, const Keyword&) = default;
	friend auto operator<=>(const Keyword&, const Keyword&) = default;

	std::string value;
};

struct Word
{
	friend bool operator==(const Word&, const Word&) = default;
	friend auto operator<=>(const Word&, const Word&) = default;

	std::string value;
};

struct Integer
{
	friend bool operator==(const Integer&, const Integer&) = default;
	friend auto operator<=>(const Integer&, const Integer&) = default;

	int value;
};

struct Number
{
	friend bool operator==(const Number&, const Number&) = default;
	friend auto operator<=>(const Number&, const Number&) = default;

	double value;
};

struct String
{
	friend bool operator==(const String&, const String&) = default;
	friend auto operator<=>(const String&, const String&) = default;

	std::string value;
	StringDelimiter delimiter;
};

struct Operator
{
	friend bool operator==(const Operator&, const Operator&) = default;
	friend auto operator<=>(const Operator&, const Operator&) = default;

	std::string value;
};

struct Comment
{
	Comment() = default;
	Comment(std::string v, std::string delim_begin, std::string delim_end):
		value(std::move(v)),
		delimiter(CommentDelimiter{ std::move(delim_begin), std::move(delim_end) })
	{}

	template <typename It>
	Comment(It begin_value, It end_value, const CommentDelimiter& delim):
		value(begin_value, end_value),
		delimiter(delim)
	{}

	friend bool operator==(const Comment&, const Comment&) = default;
	friend auto operator<=>(const Comment&, const Comment&) = default;

	std::string value;
	CommentDelimiter delimiter;
};

struct Expression
{
	friend bool operator==(const Expression&, const Expression&) = default;
	friend auto operator<=>(const Expression&, const Expression&) = default;

	std::vector<Token> value;
	std::optional<SubexpressionDelimiter> delimiter;
};

} // namespace crepuscule
