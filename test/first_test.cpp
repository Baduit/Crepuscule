#include <sstream>

#include <boost_ut/ut.hpp>

#include <crepuscule/crepuscule.hpp>

using namespace boost::ut;

void complete_pseudo_langage_tokenization()
{
	crepuscule::Config config;
	config.string_delimiters.push_back({ "\"", "\"" });
	config.string_delimiters.push_back({ "'", "'" });

	config.custom_string_sequences.push_back({ "\\n", "\n" });
	config.custom_string_sequences.push_back({ "\\t", "\t" });
	config.custom_string_sequences.push_back({ "\\\"", "\"" });
	config.custom_string_sequences.push_back({ "\\'", "'" });
	config.custom_string_sequences.push_back({ "\\\\", "\\" });

	config.operators.push_back("==");
	config.operators.push_back("!=");
	config.operators.push_back(">=");
	config.operators.push_back("<=");
	config.operators.push_back(">");
	config.operators.push_back("<");
	config.operators.push_back("+");
	config.operators.push_back("-");
	config.operators.push_back("*");
	config.operators.push_back("/");
	config.operators.push_back("%");
	config.operators.push_back("=");
	config.operators.push_back(";");

	config.delimiters.push_back(" ");
	config.delimiters.push_back("\t");
	config.delimiters.push_back("\n");

	config.keywords.push_back("if");
	config.keywords.push_back("else");
	config.keywords.push_back("while");
	config.keywords.push_back("or");
	config.keywords.push_back("and");
	config.keywords.push_back("not");
	config.keywords.push_back("var");

	config.subexpression_delimiters.push_back({ "(", ")" });
	config.subexpression_delimiters.push_back({ "{", "}" });

	config.comment_delimiters.push_back({ "//", "\n" });
	config.comment_delimiters.push_back({ "/*", "*/" });

	config.integer_reader =
		[](std::string_view str) -> std::optional<int>
		{
			for (auto c: str)
				if (!isdigit(c))
					return {};
			std::stringstream ss;
			ss << str;
			int result;
			ss >> result;
			return result;
		};

	std::string_view text =
		R"(
			// Declare some variables
			var i = 3;
			var b = 3 + 4;
			var str = "bobo\n";
			if (i == b and str == "\"")
			{
				/*
					THis is a comment
				*/
				{
					b = 0;
				}
			}
		)";
	
	auto expression = crepuscule::tokenize(text, config);
	std::cout << expression;
}

int main()
{
	"complete_pseudo_langage_tokenization"_test = complete_pseudo_langage_tokenization;
}