#pragma once

//This file defines the State Machine class 

// Include libraries
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cerr;
#include <string>
using std::string;
#include <vector>
using std::vector;

////////////////////////////////////////////////////////////////////////////////////////////
// create enumerated data types to access the array containing the Sate Trasition table	  //
////////////////////////////////////////////////////////////////////////////////////////////
enum State {
	STARTING_STATE,				// 0
	IN_IDENTIFIER,				// 1
	END_OF_IDENTIFIER,			// 2 final state
	IN_NUMBER,					// 3
	IN_FLOAT,					// 4
	END_OF_NUMBER,				// 5 final state
	IN_COMMENT,					// 6
	END_OF_COMMENT,				// 7 final state
	SYMBOLS						// 8 final state
};

enum Input {
	LETTER,			// 0
	DIGIT,			// 1
	SPACE,			// 2
	EXCLAMATION,	// 3
	DOLLAR_SIGN,	// 4
	PERIOD,			// 5
	OTHER,			// 6
	BACKUP			// 7 not an input, but a flag that tells the lexer when to back up 
};


////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
//	State machine class definition                                                        //
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
class StateMachine {


private:

	////////////////////////////////////////////////////////////////////////////////////////
	// private variables for the State Machine class
	////////////////////////////////////////////////////////////////////////////////////////
	const int NUM_OF_STATES = 9;
	const int NUM_OF_INPUTS = 8;
	const int NUM_OF_FINAL_STATES = 4;
	const int final_states[4] = { END_OF_IDENTIFIER, END_OF_NUMBER, END_OF_COMMENT, SYMBOLS };
	int state_transition_table[9][8] =
	{
		{ 1, 3, 0, 6, 8, 4, 8, 0 },
		{ 1, 1, 2, 2, 1, 2, 2, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 5, 3, 5, 5, 5, 4, 5, 0 },
		{ 5, 4, 5, 5, 5, 5, 5, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 1 },
		{ 6, 6, 6, 7, 6, 6, 6, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	// state_transition_table
	/*
	STATES				INPUTS
	--------------------L	D	SP	!	$	.	OTHER	BACKUP(not input, some final states must back up)
	0 StartingState		1	3	0	6	8	4	8		0
	1 InIdentifier		1	1	2	2	1	2	2		0
	2 EndOfIdentifier	0	0	0	0	0	0	0		1	final state
	3 InNumber			5	3	5	5	5	4	5		0
	4 InFloat			5	4	5	5	5	5	5		0
	5 EndOfNUmber		0	0	0	0	0	0	0		1	final state
	6 InComment			6	6	6	7	6	6	6		0
	7 EndOfComment		0	0	0	0	0	0	0		0	final state
	8 Symbol			0	0	0	0	0	0	0		0	final state
	*/


public:

	////////////////////////////////////////////////////////////////////////////////////////
	// function to calculate the next state based on the current state and input
	////////////////////////////////////////////////////////////////////////////////////////
	int check_input(int state, int input) {
		return state_transition_table[state][input];
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function that returns true if the lexer should back up
	////////////////////////////////////////////////////////////////////////////////////////
	bool should_back_up(int curr_state) {
		if (state_transition_table[curr_state][BACKUP] == 1)
			return 1;
		else
			return 0;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//	Function returns the name of the token based on the state and lexeme
	///////////////////////////////////////////////////////////////////////////////////////
	string getTokenName(int state, string lexeme) {

		if (state == END_OF_IDENTIFIER) {
			if (lexeme == "int" || lexeme == "float" || lexeme == "bool" || lexeme == "if" || lexeme == "else" ||
				lexeme == "then" || lexeme == "do" || lexeme == "while" || lexeme == "whileend" || lexeme == "do" ||
				lexeme == "doend" || lexeme == "for" || lexeme == "and" || lexeme == "or" || lexeme == "function") {
				return "KEYWORD";
			}
			else {
				return "IDENTIFIER";
			}
		}

		else if (state == END_OF_NUMBER) {
			return "NUMBER";
		}

		else if (state == END_OF_COMMENT) {
			return "COMMENT";
		}

		else if (state == SYMBOLS) {
			if (lexeme == "*" || lexeme == "+" || lexeme == "-" || lexeme == "=" || lexeme == "/" ||
				lexeme == ">" || lexeme == "<" /*|| lexeme == "%"*/) {
				return "OPERATOR";
			}
			else if (lexeme == "'" || lexeme == "(" || lexeme == ")" || lexeme == "{" || lexeme == "}" ||
				lexeme == "[" || lexeme == "]" || lexeme == "," || lexeme == "." || lexeme == ":" || lexeme == ";" ||
				lexeme == "!") {
				return "SEPARATOR";
			}
			else
				return "OTHER";
		}
		else
			return "ERROR";
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function that takes the source code char and converts it to an input for the table
	////////////////////////////////////////////////////////////////////////////////////////
	int char_to_input(char code) {
		if (isalpha(code))
			return LETTER;
		else if (isdigit(code))
			return DIGIT;
		else if (isspace(code))
			return SPACE;
		else if (code == '!')
			return EXCLAMATION;
		else if (code == '$')
			return DOLLAR_SIGN;
		else if (code == '.')
			return PERIOD;
		else
			return OTHER;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function checks if current state is a final state
	////////////////////////////////////////////////////////////////////////////////////////
	bool is_final_state(int curr_state) {
		for (int i = 0; i < NUM_OF_FINAL_STATES; i++) {
			if (curr_state == final_states[i])
				return 1;
		}
		return 0;
	}

};