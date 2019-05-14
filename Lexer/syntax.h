#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;
#include <fstream>
using std::ofstream;
using std::ifstream;
#include <assert.h>
#include <iomanip>
using std::setw;
using std::right;
using std::left;
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
using std::cin;


// Define token structure. This holds tokens and lexemes
struct tokens {
	string token;
	string lexeme;
	tokens(string tok, string lex) {
		token = tok, lexeme = lex;
	}
};

// function prototypes 
bool analyze_syntax(vector<tokens>&, ofstream&);
int string_to_index(string);
void print_rule(string, string, ofstream&);

// Function uses a predictive table to analyze syntax
bool analyze_syntax(vector<tokens>& token_vect, ofstream& output_file) {

	vector<vector<string>> predictive_table = {
	//			i    =     +     -      *     /      (       )      $ 
	/*S*/	{ "i=E", "", ""   , ""   , ""   , ""   , ""   , ""   , "" },
	/*E*/	{ "TQ" , "", ""   , ""   , ""   , ""   , "TQ" , ""   , "" },
	/*Q*/	{ ""   , "", "+TQ", "-TQ", ""   , ""   , ""   , "e"  , "e" },
	/*R*/	{ "FR" , "", ""   , ""   , ""   , ""   , "FR" , ""   , "" },
	/*T*/	{ ""   , "", "e"  , "e"  , "*FR", "/FR", ""   , "e"  , "e" },
	/*F*/	{ "i"  , "", ""   , ""   , ""   , ""   , "(E)", ""   , "" }
	};
	vector<string> stack;
	vector<tokens> line;

	for (vector<tokens>::iterator it = token_vect.begin();  it != token_vect.end(); it++)
	{
		if (it->lexeme != ";")
		{
			line.push_back(tokens(it->token, it->lexeme));
		}
		else 
		{
			stack.push_back("$");
			line.push_back(tokens("$", "$"));
			stack.push_back("S");
			vector<tokens>::iterator current = line.begin();
			string top_of_stack;
			string token;

			cout << endl << "------------------------------------------" << endl;
			output_file << endl << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;
			output_file << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;

			while (!stack.empty())
			{
				top_of_stack = stack.back();
				if (current->token == "IDENTIFIER") { token = "i"; }
				else { token = current->lexeme; }

				if (top_of_stack == "i" || top_of_stack == "=" || top_of_stack == "+" ||
					top_of_stack == "-" || top_of_stack == "*" || top_of_stack == "/" ||
					top_of_stack == "(" || top_of_stack == ")" || top_of_stack == "$")
				{
					if (top_of_stack == token)
					{
						stack.pop_back();
						current++;
						if ((!stack.empty()) && current->lexeme != "$")
						{
							cout << endl << "------------------------------------------" << endl;
							output_file << endl << "------------------------------------------" << endl;
							cout << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
							output_file << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
						}
					}
					else
					{
						return false;
					}
				}
				else
				{
					assert(string_to_index(top_of_stack) != -1);
					assert(string_to_index(token) != -1);
					string prod_rule = predictive_table[string_to_index(top_of_stack)][string_to_index(token)];
					if (!prod_rule.empty())
					{
						print_rule(top_of_stack, prod_rule, output_file);
						stack.pop_back();
						while (!prod_rule.empty())
						{
							if (prod_rule != "e") {
								stack.push_back(string(1, prod_rule.back()));
							}
							prod_rule.pop_back();
						}

					}
					else
					{
						return false;
					}
				}
			}
			cout << endl << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			cout << "<Empty> -> <Epsilon>" << endl;
			output_file << endl << "------------------------------------------" << endl;
			output_file << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			output_file << "<Empty> -> <Epsilon>" << endl;
			
			line.clear();
		}
	}
	return true;
}




int string_to_index(string word)
{
	if (word == "S" || word == "i") { return 0; }
	else if (word == "E" || word == "=") { return 1; }
	else if (word == "Q" || word == "+") { return 2; }
	else if (word == "T" || word == "-") { return 3; }
	else if (word == "R" || word == "*") { return 4; }
	else if (word == "F" || word == "/") { return 5; }
	else if (word == "(") { return 6; }
	else if (word == ")") { return 7; }
	else if (word == "$") { return 8; }
	else return -1;
}

// Functon prints the production rules used to standard output and the output file 
void print_rule(string statement, string prod_rule, ofstream& output_file)
{
	if (statement == "S")
	{
		if (prod_rule == "i=E")
		{
			cout << "<Statement> -> Identifier = <Expression>" << endl;
			output_file << "<Statement> -> Identifier = <Expression>" << endl;
		}
	}

	else if (statement == "E")
	{
		if (prod_rule == "TQ")
		{
			cout << "<Expression> -> <Term> <Expression Prime>" << endl;
			output_file << "<Expression> -> <Term> <Expression Prime>" << endl;
		}
	}

	else if (statement == "Q")
	{
		if (prod_rule == "+TQ")
		{
			cout << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
			output_file << "<Expression Prime> -> + <Term> <Expression Prime>" << endl;
		}
		else if (prod_rule == "-TQ")
		{
			cout << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
			output_file << "<Expression Prime> -> - <Term> <Expression Prime>" << endl;
		}
		if (prod_rule == "e")
		{
			cout << "<Expression Prime> -> <Epsilon>" << endl;
			output_file << "<Expression Prime> -> <Epsilon>" << endl;
		}
	}

	else if (statement == "T")
	{
		if (prod_rule == "FR")
		{
			cout << "<Term> -> <Factor> <Term Prime>" << endl;
			output_file << "<Term> -> <Factor> <Term Prime>" << endl;
		}
	}

	else if (statement == "R")
	{
		if (prod_rule == "*FR")
		{
			cout << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
			output_file << "<Term Prime> -> * <Factor> <Term Prime>" << endl;
		}
		else if (prod_rule == "/FR")
		{
			cout << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
			output_file << "<Term Prime> -> / <Factor> <Term Prime>" << endl;
		}
		else if (prod_rule == "e")
		{
			cout << "<Term Prime> -> <Epsilon>" << endl;
			output_file << "<Term Prime> -> <Epsilon>" << endl;
		}
	}

	else if (statement == "F")
	{
		if (prod_rule == "i")
		{
			cout << "<Factor> -> Identifier" << endl;
			output_file << "<Factor> -> Identifier" << endl;
		}
		else if (prod_rule == "(E)")
		{
			cout << "<Factor> -> ( <Expression> )" << endl;
			output_file << "<Factor> -> ( <Expression> )" << endl;
		}
	}
}
