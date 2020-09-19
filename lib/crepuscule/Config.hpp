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
	bool operator==(const SubexpressionDelimiter& other) const // If I default it it segfault, gcc bug ?
	{
		return begin == other.begin && end == other.end;
	}

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
	// Add a method to be sure all list are sorted in a way the bigger element
	// to find in the input str is at the beginning of the vector
	// Or maybe just use sets instead of vector ?
	// An other solution would be to have a class Tokenizer with the config as attribute
	// so it can have the whole control over theses data
	// I'm still in R&D phase

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
