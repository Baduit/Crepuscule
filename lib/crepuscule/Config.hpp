#pragma once

#include <vector>
#include <iostream>
#include <compare>
#include <functional>
#include <string>
#include <string_view>
#include <optional>

namespace crepuscule
{

struct StringDelimiter
{
	bool operator==(const StringDelimiter&) const = default;
	auto operator<=>(const StringDelimiter& other) const
	{
		// Only the begin matters when sorting
		return begin <=> other.begin;
	}

	std::string begin;
	std::string end;
};

struct CustomStringSequence
{
	bool operator==(const CustomStringSequence&) const = default;
	auto operator<=>(const CustomStringSequence& other) const
	{
		// Only the sequence matters when sorting
		return sequence <=> other.sequence;
	}

	std::string sequence;
	std::string replacement;
};

struct SubexpressionDelimiter
{
	 // If I default it it segfault (infinite loop of calling this operator making it stackoverflow), gcc bug ?
	bool operator==(const SubexpressionDelimiter& other) const
	{
		return begin == other.begin && end == other.end;
	}

	auto operator<=>(const SubexpressionDelimiter& other) const
	{
		// Only the begin matters when sorting
		return begin <=> other.begin;
	}

	std::string begin;
	std::string end;
};

struct CommentDelimiter
{
	bool operator==(const CommentDelimiter&) const = default;
	auto operator<=>(const CommentDelimiter& other) const
	{
		// Only the begin matters when sorting
		return begin <=> other.begin;
	}

	std::string begin;
	std::string end;
};

struct Config
{
	// Add a way to check for incoherencies ?
	std::vector<StringDelimiter> string_delimiters;
	std::vector<CustomStringSequence> custom_string_sequences;
	std::vector<std::string> operators;
	std::vector<std::string> delimiters;
	std::vector<std::string> keywords;
	std::vector<SubexpressionDelimiter> subexpression_delimiters;
	std::vector<CommentDelimiter> comment_delimiters;

	std::function<std::optional<int> (std::string_view)> integer_reader = nullptr;
	std::function<std::optional<double> (std::string_view)> number_reader = nullptr;
};

} // namespace crepuscule
