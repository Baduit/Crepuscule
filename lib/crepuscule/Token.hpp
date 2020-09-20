#pragma once

#include <variant>
#include <optional>
#include <compare>

#include <crepuscule/Config.hpp>

namespace crepuscule
{

constexpr double DOUBLE_PRECISION_COMPARAISON = 0.0000001;

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
	Keyword() = default;
	Keyword(std::string str):
		value(std::move(str))
	{}

	friend bool operator==(const Keyword&, const Keyword&) = default;

	std::string value;
};

struct Word
{
	Word() = default;
	Word(std::string str):
		value(std::move(str))
	{}

	friend bool operator==(const Word&, const Word&) = default;

	std::string value;
};

struct Integer
{
	constexpr Integer() = default;
	constexpr Integer(int i):
		value(i)
	{}

	friend constexpr bool operator==(const Integer&, const Integer&) = default;

	int value = 0;
};

struct Number
{
	constexpr Number() = default;
	constexpr Number(double n):
		value(n)
	{}

	friend constexpr bool operator==(const Number& a, const Number& b)
	{
		return
			(a.value + DOUBLE_PRECISION_COMPARAISON) > b.value &&
			(a.value - DOUBLE_PRECISION_COMPARAISON) < b.value;
	}

	double value = 0.0;
};

struct String
{
	String() = default;
	String(StringDelimiter delim):
		value(),
		delimiter(std::move(delim))
	{}
	String(std::string str, StringDelimiter delim):
		value(std::move(str)),
		delimiter(std::move(delim))
	{}

	friend bool operator==(const String&, const String&) = default;

	std::string value;
	StringDelimiter delimiter;
};

struct Operator
{
	Operator() = default;
	Operator(std::string str):
		value(std::move(str))
	{}

	friend bool operator==(const Operator&, const Operator&) = default;

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

	std::string value;
	CommentDelimiter delimiter;
};

struct Expression
{
	Expression() = default;
	Expression(std::vector<Token> t):
		value(std::move(t))
	{}
	Expression(std::vector<Token> t, SubexpressionDelimiter delim):
		value(std::move(t)),
		delimiter(std::move(delim))
	{}
	Expression(SubexpressionDelimiter delim):
		value(),
		delimiter(std::move(delim))
	{}

	friend bool operator==(const Expression&, const Expression&) = default;

	std::vector<Token> value;
	std::optional<SubexpressionDelimiter> delimiter;
};

} // namespace crepuscule
