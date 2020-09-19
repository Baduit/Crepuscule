#pragma once

#include <string>
#include <string_view>
#include <algorithm>
#include <concepts>
#include <vector>
#include <optional>

namespace crepuscule
{

namespace helpers
{

template <typename T>
concept Delimiter = 
	requires(T delim)
	{
		{ delim.begin };
		{ delim.end };
	};


template <Delimiter D>
auto find_delimiter_begin(const std::vector<D>& delimiters, std::string_view str)
{
	return std::find_if(delimiters.begin(), delimiters.end(),
		[&](const auto& d)
		{
			return str.starts_with(d.begin);
		});
}

template <typename T>
concept CustomSequence = 
	requires(T seq)
	{
		{ seq.sequence };
		{ seq.replacement };
	};

template <CustomSequence CS>
auto find_custom_sequence(const std::vector<CS>& custom_sequences, std::string_view str)
{
	return std::find_if(custom_sequences.begin(), custom_sequences.end(),
		[&](const auto& cs)
		{
			return str.starts_with(cs.sequence);
		});
}

inline auto find_at_start(const std::vector<std::string>& elems, std::string_view str)
{
	return std::find_if(elems.begin(), elems.end(),
		[&](const auto& s)
		{
			return str.starts_with(s);
		});
}

// Not ideal but fuck it 
template <typename It>
std::optional<It> to_optional_it(It found, It end)
{
	// TODO: investigate why if I make return {}
	// or return std::optional<It>()
	// its says it is maybe uninitialized
	std::optional<It> opt;
	if (found == end)
		return opt;
	else
		return found;
}

} // namespace helpers


} // namespace crepuscule
