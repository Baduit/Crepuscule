#pragma once

#include <vector>
#include <compare>
#include <functional>
#include <string>
#include <string_view>

namespace crepuscule
{

struct StringDelimiter
{
	bool operator==(const StringDelimiter&) const = default;
	auto operator<=>(const StringDelimiter&) const = default;

	std::string begin;
	std::string end;
};

struct CustomStringSequence
{
	bool operator==(const CustomStringSequence&) const = default;
	auto operator<=>(const CustomStringSequence&) const = default;

	std::string sequence;
	std::string replacement;
};

struct SubexpressionDelimiter
{
	bool operator==(const SubexpressionDelimiter&) const = default;
	auto operator<=>(const SubexpressionDelimiter&) const = default;

	std::string begin;
	std::string end;
};

struct CommentDelimiter
{
	bool operator==(const CommentDelimiter&) const = default;
	auto operator<=>(const CommentDelimiter&) const = default;

	std::string begin;
	std::string end;
};

struct Config
{
	std::vector<StringDelimiter> string_delimiters;
	std::vector<CustomStringSequence> custom_string_sequences;
	std::vector<std::string> operators;
	std::vector<std::string> delimiters;
	std::vector<std::string> keywords;
	std::vector<SubexpressionDelimiter> subexpression_delimiters;
	std::vector<std::string> line_comment_start;
	std::vector<CommentDelimiter> comment_delimiters;

	std::function<int (std::string_view)> integer_reader = nullptr;
	std::function<double (std::string_view)> number_reader = nullptr;
};

} // namespace crepuscule
