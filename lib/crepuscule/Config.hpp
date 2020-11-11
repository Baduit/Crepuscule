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
namespace 
{

// Because not all compiler are up to date with the <=> with std::string
inline std::strong_ordering compare_string(const std::string& a, const std::string& b)
{
	if (a == b)
		return std::strong_ordering::equal;
	else if (a < b)
		return std::strong_ordering::less;
	else
		return std::strong_ordering::greater;
}

} // namespace 



struct StringDelimiter
{
	bool operator==(const StringDelimiter&) const = default;
	bool operator>(const StringDelimiter& other) const
	{
		return begin > other.begin;
	}
	// MSVC is not ready with <=>
	/* auto operator<=>(const StringDelimiter& other) const
	{
		// Only the begin matters when sorting
		return compare_string(begin, other.begin);
	} */

	std::string begin;
	std::string end;
};

struct CustomStringSequence
{
	bool operator==(const CustomStringSequence&) const = default;
	// MSVC is not ready with <=>
	/* auto operator<=>(const CustomStringSequence& other) const
	{
		// Only the sequence matters when sorting
		return compare_string(sequence, other.replacement);
	} */
	bool operator>(const CustomStringSequence& other) const
	{
		return sequence > other.sequence;
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

	bool operator>(const SubexpressionDelimiter& other) const
	{
		return begin > other.begin;
	}
	// MSVC is not ready with <=>
	/* auto operator<=>(const SubexpressionDelimiter& other) const
	{
		// Only the begin matters when sorting
		return compare_string(begin, other.begin);
	} */

	std::string begin;
	std::string end;
};

struct CommentDelimiter
{
	bool operator==(const CommentDelimiter&) const = default;
	bool operator>(const CommentDelimiter& other) const
	{
		return begin > other.begin;
	}
	// MSVC is not ready with <=>
	/* auto operator<=>(const CommentDelimiter& other) const
	{
		// Only the begin matters when sorting
		return compare_string(begin, other.begin);
	} */

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
