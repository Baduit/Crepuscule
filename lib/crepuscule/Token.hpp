#pragma once

#include <variant>
#include <optional>

#include <crepuscule/Config.hpp>

namespace crepuscule
{

struct Keyword;
struct Word;
struct Integer;
struct Number;
struct String;
struct Comment;
struct MultilineComment;
struct Expression;
struct Operator;

using Token = std::variant<Keyword, Word, Integer, Number, String, Operator, Comment, MultilineComment, Expression>;

/*
	TODO: instead of making a deep copy of the delimiters I could:
	use a shared_ptr and always use a pointer to the same instance
	=> easy to use, hard to use the wrong way and probably faster (benchmark needed)
*/


struct Keyword
{
	std::string value;
};

struct Word
{
	std::string value;
};

struct Integer
{
	int value;
};

struct Number
{
	double value;
};

struct String
{
	std::string value;
	StringDelimiter delimiter;
};

struct Operator
{
	std::string value;
};

struct Comment
{
	std::string value;
	std::string start;
};

struct MultilineComment
{
	MultilineComment() = default;

	template <typename It>
	MultilineComment(It begin_value, It end_value, const CommentDelimiter& delim):
		value(begin_value, end_value),
		delimiter(delim)
	{}

	std::string value;
	CommentDelimiter delimiter;
};

struct Expression
{
	std::vector<Token> value;
	std::optional<SubexpressionDelimiter> delimiter;
};

} // namespace crepuscule
