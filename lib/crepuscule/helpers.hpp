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
	return std::find_if(delimiters.cbegin(), delimiters.cend(),
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
	return std::find_if(custom_sequences.cbegin(), custom_sequences.cend(),
		[&](const auto& cs)
		{
			return str.starts_with(cs.sequence);
		});
}

inline auto find_at_start(const std::vector<std::string>& elems, std::string_view str)
{
	return std::find_if(elems.cbegin(), elems.cend(),
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

template <typename It>
std::string_view range_to_string_view(It begin, It end)
{
	// MSVC does not support this for now
	//return std::string_view(_it_input, _it_end);
	auto size = static_cast<std::size_t>(end - begin);
	return std::string_view(&(*begin), size);
}

} // namespace helpers


} // namespace crepuscule
