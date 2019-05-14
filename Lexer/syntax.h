#pragma once

// This file contains functions to anlyze the syntax of the input file 

// Include libraries 
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

// Function uses a predictive table and stack to analyze syntax
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
	vector<tokens> string_line;

	// Look at every token in the file 
	for (vector<tokens>::iterator it = token_vect.begin();  it != token_vect.end(); it++)
	{
		// Separate tokens into lines of code 
		if (it->lexeme != ";")
		{
			// If this isn't end of a line add token to the string
			string_line.push_back(tokens(it->token, it->lexeme));
		}

		// If we reach the end of the line ';' analyze the string 
		else 
		{
			// Set up the Stack with the ending symbol '$' and the starting symbol 'S'
			stack.push_back("$");
			stack.push_back("S");

			// push the ending symbol '$' to the string
			string_line.push_back(tokens("$", "$"));

			// get a pointer to the current token that's being analyzed
			vector<tokens>::iterator current = string_line.begin();
			string top_of_stack;
			string token;

			// Print token and lexeme that are currently being analyzed
			cout << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;
			output_file << "------------------------------------------" << endl;
			output_file << "Token: " << left << setw(25) << current->token <<
				"Lexeme: " << current->lexeme << endl;

			// Repeat until stack is empty
			while (!stack.empty())
			{
				// look at the top of the stack and the current token
				top_of_stack = stack.back();
				if (current->token == "IDENTIFIER") { token = "i"; }
				else { token = current->lexeme; }

				// If the top of the stack is a Terminal symbol
				if (top_of_stack == "i" || top_of_stack == "=" || top_of_stack == "+" ||
					top_of_stack == "-" || top_of_stack == "*" || top_of_stack == "/" ||
					top_of_stack == "(" || top_of_stack == ")" || top_of_stack == "$")
				{
					// Verify that current token matches symbol on top of the stack
					if (top_of_stack == token)
					{
						// Pop the Stack and advance the pointer to the next token
						stack.pop_back();
						current++;

						// Print new token and lexeme to be analyzed
						if ((!stack.empty()) && current->lexeme != "$")
						{
							cout << endl << "------------------------------------------" << endl;
							cout << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
							output_file << endl << "------------------------------------------" << endl;
							output_file << "Token: " << left << setw(25) << current->token <<
								"Lexeme: " << current->lexeme << endl;
						}
					}
					// Syntax error if token and top of the stack don't match
					else
					{
						return false;
					}
				}

				// If the top of the stack is a Non Terminal
				else
				{
					// Look at the production table to get the new production rule 
					assert(string_to_index(top_of_stack) != -1);
					assert(string_to_index(token) != -1);
					string prod_rule = predictive_table[string_to_index(top_of_stack)][string_to_index(token)];

					// If there is a production rule for the symbol on top of the stack and the current token
					if (!prod_rule.empty())
					{
						// Print production rule used 
						print_rule(top_of_stack, prod_rule, output_file);

						// Pop stack, push new production rule to stack in reverse order, or nothing if epsilon
						stack.pop_back();
						while (!prod_rule.empty())
						{
							if (prod_rule != "e") {	stack.push_back(string(1, prod_rule.back())); }
							prod_rule.pop_back();
						}
					}

					// If the predictive table is empty, syntax is invlaid
					else
					{
						return false;
					}
				}
			}
			// Print token and lexeme at the end of the string 
			cout << endl << "------------------------------------------" << endl;
			cout << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			cout << "<Empty> -> <Epsilon>" << endl << endl;
			output_file << endl << "------------------------------------------" << endl;
			output_file << "Token: " << left << setw(25) << it->token <<
				"Lexeme: " << it->lexeme << endl;
			output_file << "<Empty> -> <Epsilon>" << endl << endl;
			
			// Clear string being analyzed before reading next line
			string_line.clear();
		}
	}
	// If the stack is empty and there are no more tokens to analyze syntax is accepted
	return true;
}


// Function that converts Terminals and Non Terminals to indexes for the predictive table
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
