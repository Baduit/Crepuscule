#pragma once

#include <variant>
#include <optional>
#include <compare>
#include <limits>

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
	TODO: No deep copies of the delimiters, use view over something stored in the result
*/

constexpr std::size_t INVALID_LINE_NUMBER = std::numeric_limits<std::size_t>::max();

struct Keyword
{
	Keyword() = default;
	Keyword(std::string str, std::size_t l):
		value(std::move(str)),
		line(l)
	{}

	friend bool operator==(const Keyword&, const Keyword&) = default;

	std::string value;
	std::size_t line = INVALID_LINE_NUMBER;
};

struct Word
{
	Word() = default;
	Word(std::string str, std::size_t l):
		value(std::move(str)),
		line(l)
	{}

	friend bool operator==(const Word&, const Word&) = default;

	std::string value;
	std::size_t line = INVALID_LINE_NUMBER;
};

struct Integer
{
	constexpr Integer() = default;
	constexpr Integer(int i, std::size_t l):
		value(i),
		line(l)
	{}

	friend constexpr bool operator==(const Integer&, const Integer&) = default;

	int value = 0;
	std::size_t line = INVALID_LINE_NUMBER;
};

struct Number
{
	constexpr Number() = default;
	constexpr Number(double n, std::size_t l):
		value(n),
		line(l)
	{}

	friend constexpr bool operator==(const Number& a, const Number& b)
	{
		return
			(a.value + DOUBLE_PRECISION_COMPARAISON) > b.value &&
			(a.value - DOUBLE_PRECISION_COMPARAISON) < b.value;
	}

	double value = 0.0;
	std::size_t line = INVALID_LINE_NUMBER;
};

struct String
{
	String() = default;
	String(StringDelimiter delim, std::size_t l):
		value(),
		delimiter(std::move(delim)),
		line(l)
	{}
	String(std::string str, StringDelimiter delim, std::size_t l):
		value(std::move(str)),
		delimiter(std::move(delim)),
		line(l)
	{}

	friend bool operator==(const String&, const String&) = default;

	std::string value;
	StringDelimiter delimiter;
	std::size_t line = INVALID_LINE_NUMBER;
};

struct Operator
{
	Operator() = default;
	Operator(std::string str, std::size_t l):
		value(std::move(str)),
		line(l)
	{}

	friend bool operator==(const Operator&, const Operator&) = default;

	std::string value;
	std::size_t line = INVALID_LINE_NUMBER;
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
