#include <iostream>

#include <crepuscule/crepuscule.hpp>

void other()
{
	crepuscule::Config config;

	config.subexpression_delimiters.push_back({ "[[", "]]" });
	config.subexpression_delimiters.push_back({ "==", "==" });
	config.subexpression_delimiters.push_back({ "'''", "'''" });
	config.subexpression_delimiters.push_back({ "''", "''" });
	config.subexpression_delimiters.push_back({ "{{", "}}" });
	config.subexpression_delimiters.push_back({ "<ref>", "</ref>" });
	config.subexpression_delimiters.push_back({ "[[", "]]" });

	config.keywords.emplace_back("File:");
	config.keywords.emplace_back("Category:");

	std::string_view text =
		"First line\n"
		"Second line\n"
		"Toto is fame";

	crepuscule::Tokenizer tokenizer(config);
}

int main()
{
	std::cout << "Before" << std::endl;
	other();
	std::cout << "after" << std::endl;
}