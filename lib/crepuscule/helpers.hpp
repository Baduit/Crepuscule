#pragma once

#include <string>
#include <string_view>
#include <algorithm>
#include <concepts>
#include <vector>

namespace crepuscule
{

namespace helpers
{

template <typename T>
concept Delimiter = 
	requires(T delim)
	{
		{ delim.begin } -> std::same_as<std::string>;
		{ delim.end } -> std::same_as<std::string>;
	};


template <Delimiter D>
auto find_delimiter_begin(const std::vector<D>& delimiters, std::string_view begin_to_find)
{
	return std::find(delimiters.begin, delimiters.end,
		[&](const auto& d)
		{
			return d.begin == begin_to_find;
		});
}

template <typename T>
concept CustomSequence = 
	requires(T seq)
	{
		{ seq.sequence } -> std::same_as<std::string>;
		{ seq.replacement } -> std::same_as<std::string>;
	};

template <CustomSequence CS>
auto find_custom_sequence(const std::vector<CS>& custom_sequences, std::string_view cs_to_find)
{
	return std::find(custom_sequences.begin, custom_sequences.end,
		[&](const auto& cs)
		{
			return cs.sequence == cs_to_find;
		});
}

} // namespace helpers


} // namespace crepuscule
