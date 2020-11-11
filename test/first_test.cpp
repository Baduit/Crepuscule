#include <iostream>
#include <utility>
#include <optional>

#include <boost_ut/ut.hpp>

#include <crepuscule/crepuscule.hpp>
#include <crepuscule/TokenPrinter.hpp>

using namespace boost::ut;
using namespace crepuscule;

void complete_pseudo_langage_tokenization()
{
	Config config;
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


	auto result = Tokenizer(config)(text);
	auto& expression = result.expression;

	expect((!expression.delimiter) >> fatal) << "The main expression does not have delimiter";
	expect(expression.value.size() == 21) << "Check number of tokens";

	// 1st line
	expect((expression.value.size() > 0) >> fatal);
	expect(expression.value[0] == Token(Comment(" Declare some variables", "//", "\n"))) << "First line is a comment";

	// 1st assigment
	expect((expression.value.size() > 5) >> fatal);
	expect(expression.value[1] == Token(Keyword("var", 3))) << "First assigment: keyword var";
	expect(expression.value[2] == Token(Word("i", 3))) << "First assigment: word i";
	expect(expression.value[3] == Token(Operator("=", 3))) << "First assigment: operator =";
	expect(expression.value[4] == Token(Integer(3, 3))) << "First assigment: integer 3";
	expect(expression.value[5] == Token(Operator(";", 3))) << "First assigment: operator ;";

	// 2nd assigment
	expect((expression.value.size() > 12) >> fatal);
	expect(expression.value[6] == Token(Keyword("var", 4))) << "Second assigment: keyword var";
	expect(expression.value[7] == Token(Word("b", 4))) << "Second assigment: word b";
	expect(expression.value[8] == Token(Operator("=", 4))) << "Second assigment: operator =";
	expect(expression.value[9] == Token(Integer(3, 4))) << "Second assigment: integer 3";
	expect(expression.value[10] == Token(Operator("+", 4))) << "Second assigment: operator +";
	expect(expression.value[11] == Token(Integer(4, 4))) << "Second assigment: integer 4";
	expect(expression.value[12] == Token(Operator(";", 4))) << "Second assigment: operator ;";

	// 3rd assigment
	expect((expression.value.size() > 17) >> fatal);
	expect(expression.value[13] == Token(Keyword("var", 5))) << "Third assigment: keyword var";
	expect(expression.value[14] == Token(Word("str", 5))) << "Third assigment: word str";
	expect(expression.value[15] == Token(Operator("=", 5))) << "Third assigment: operator =";
	expect(expression.value[16] == Token(String("bobo\n", { "\"", "\"" }, 5))) << "Third assigment: string \"bobo\\n\"";
	expect(expression.value[17] == Token(Operator(";", 5))) << "Third assigment: operator ;";

	// Condition
	expect((expression.value.size() > 19) >> fatal);
	expect(expression.value[18] == Token(Keyword("if", 6))) << "Condition: keyword if";
	auto* condition_expression = std::get_if<Expression>(&(expression.value[19]));
	expect((condition_expression) >> fatal) << "This is an expression";
	expect((condition_expression->delimiter.has_value()) >> fatal) << "The delimiter are ()";
	expect(condition_expression->delimiter.value() == SubexpressionDelimiter{"(", ")"}) << "The delimiter are ()";
	expect(condition_expression->value.size() == 7) << "Enough token in the expression";
	expect(condition_expression->value[0] == Token(Word("i", 6))) << "Condition: word i";
	expect(condition_expression->value[1] == Token(Operator("==", 6))) << "Condition: operator ==";
	expect(condition_expression->value[2] == Token(Word("b", 6))) << "Condition: word b";
	expect(condition_expression->value[3] == Token(Keyword("and", 6))) << "Condition: keyword and";
	expect(condition_expression->value[4] == Token(Word("str", 6))) << "Condition: word str";
	expect(condition_expression->value[5] == Token(Operator("==", 6))) << "Condition: operator ==";
	expect(condition_expression->value[6] == Token(String("\"", { "\"", "\"" }, 6))) << "Condition string \"\"\"";

	// Conditionnaly executed part
	expect((expression.value.size() > 20) >> fatal);
	auto* conditionnaly_executed_part = std::get_if<Expression>(&(expression.value[20]));
	expect((conditionnaly_executed_part) >> fatal) << "This is an expression";
	expect(conditionnaly_executed_part->value[0] == Token(Comment("\n\t\t\t\t\tTHis is a comment\n\t\t\t\t", "/*", "*/"))) << "Conditionnaly executed part: comment";
	auto* subexpression = std::get_if<Expression>(&(conditionnaly_executed_part->value[1]));
	expect((subexpression) >> fatal) << "There is a subexpression";
	expect(subexpression->value[0] == Token(Word("b", 12))) << "subexpression: word b";
	expect(subexpression->value[1] == Token(Operator("=", 12))) << "subexpression: operator =";
	expect(subexpression->value[2] == Token(Integer(0, 12))) << "subexpression: integer 0";
}

void test_lines()
{
	{
		std::vector<std::string_view> lines = 
			{
				"toto\n",
				"tata\n",
				"zefz zefr ze ze rze zer \n",
				"zerze r\\n ezfze zerzer\n",
				"ergerg"
			};

		std::string text;
		for (auto l: lines)
			text += l;

		auto result = Tokenizer({})(text);

		expect((result.lines.size() == lines.size()) >> fatal) << "Check number of lines";

		for (std::size_t i = 0; i < lines.size(); ++i)
			expect(result.lines[i] == lines[i]) << ("Check line individually: " + std::to_string(i));
	}

	{
		std::vector<std::string_view> lines = 
			{
				"toto\n",
				"tata\n",
				"zefz zefr ze ze rze zer \n",
				"zerze r\\n ezfze zerzer\n",
			};

		std::string text;
		for (auto l: lines)
			text += l;

		auto result = Tokenizer({})(text);

		expect((result.lines.size() == lines.size()) >> fatal) << "Check number of lines";

		for (std::size_t i = 0; i < lines.size(); ++i)
			expect(result.lines[i] == lines[i]) << ("Check line individually: " + std::to_string(i));
	}
}

void other()
{
	Config config;

	config.subexpression_delimiters.push_back({ "[[", "]]" });
	config.subexpression_delimiters.push_back({ "==", "==" });
	config.subexpression_delimiters.push_back({ "'''", "'''" });
	config.subexpression_delimiters.push_back({ "''", "''" });
	config.subexpression_delimiters.push_back({ "{{", "}}" });
	config.subexpression_delimiters.push_back({ "<ref>", "</ref>" });
	config.subexpression_delimiters.push_back({ "[[", "]]" });

	config.keywords.emplace_back("File:");
	config.keywords.emplace_back("Category:");

	/* std::string_view text =
		"First line\n"
		"Second line\n"
		"Toto is fame"; */

	Tokenizer t(config);
}

int main()
{
	"complete_pseudo_langage_tokenization"_test = complete_pseudo_langage_tokenization;
	"lines"_test = test_lines;
	"other"_test = other;
}