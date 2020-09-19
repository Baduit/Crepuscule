#pragma once

#include <iostream>

#include <crepuscule/Token.hpp>

namespace crepuscule
{

inline std::ostream& operator<<(std::ostream& os, const Keyword& k)
{
	os << "Keyword: " << k.value << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Word& w)
{
	os << "Word: " << w.value << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Integer& i)
{
	os << "Integer: " << i.value << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Number& n)
{
	os << "Number: " << n.value << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const String& s)
{
	os << "String: " << s.delimiter.begin << s.value << s.delimiter.end << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Operator& o)
{
	os << "Operator: " << o.value << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Comment& c)
{
	os << "Comment: " << c.delimiter.begin << c.value << c.delimiter.end << "\n";
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Expression& e)
{
	os << "Expression: ";
	if (e.delimiter)
		os << e.delimiter->begin;

	for (const auto& token: e.value)
	{
		std::visit(
			[&](const auto& t)
			{
				os << "\t" << t;
			}, token);
	}

	if (e.delimiter)
		os << e.delimiter->end;
	os << "\n";
	return os;
}

} // namespace crepuscule
