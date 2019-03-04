//This file defines the State Machine class 

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
	// private variables for the State Machine class									  //
	////////////////////////////////////////////////////////////////////////////////////////
	const int NUM_OF_STATES = 9;
	const int NUM_OF_INPUTS = 8;
	const int NUM_OF_FINAL_STATES = 4;
	const int final_states[4] = { END_OF_IDENTIFIER, END_OF_NUMBER, END_OF_COMMENT, SYMBOLS };
	int state_transition_table[9][8];


public:

	////////////////////////////////////////////////////////////////////////////////////////
	// StateMacine constructor initializes the State_transition_table                     //
	////////////////////////////////////////////////////////////////////////////////////////
	StateMachine() {
		// open text file to get values inside State transition table
		vector<string> vals;
		ifstream table;
		string value;
		table.open("transition_table.txt");
		if (table.is_open()) {
			while (getline(table, value)) {
				vals.push_back(value);
			}
		}
		else
			cerr << "Could not open transition_table.txt\n";

		//assign the values from the temporary vector to the state_transition_table array 
		for (int state = 0, vect_val = 0; state < NUM_OF_STATES; state++) {
			for (int input = 0; input < NUM_OF_INPUTS; input++) {
				state_transition_table[state][input] = std::stoi(vals[vect_val]);
				vect_val++;
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function to calculate the next state based on the current state and input          //
	////////////////////////////////////////////////////////////////////////////////////////
	int check_input(int state, int input) {
		return state_transition_table[state][input];
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function that returns true if the lexer should back up							  //
	////////////////////////////////////////////////////////////////////////////////////////
	bool should_back_up(int curr_state) {
		if (state_transition_table[curr_state][BACKUP] == 1)
			return 1;
		else
			return 0;
	}


	////////////////////////////////////////////////////////////////////////////////////////
	///// Function returns the type of the final state  //
	////////////////////////////////////////////////////////////////////////////////////////
	string getFinalState(int state) {
		
		if (state == END_OF_IDENTIFIER)
			return "Identifier";
		else if (state == END_OF_NUMBER)
			return "Number";
		else if (state == END_OF_COMMENT)
			return "Comment";
		else if (state == SYMBOLS)
			return "Symbol";
	}


	////////////////////////////////////////////////////////////////////////////////////////
	//	Function returns the name of the token based on the state
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
				lexeme == ">" || lexeme == "<" || lexeme == "%") {
				return "OPERATOR";
			}
			else if (lexeme == "'" || lexeme == "(" || lexeme == ")" || lexeme == "{" || lexeme == "}" ||
				lexeme == "[" || lexeme == "]" || lexeme == "," || lexeme == "." || lexeme == ":" || lexeme == ";" ||
				lexeme == "!") {
				return "SEPARATOR";
			}
			else
				return "SPACE";
		}
		else
			return "ERROR";
	}


	////////////////////////////////////////////////////////////////////////////////////////
	// function that takes the source code char and converts it to an input for the table //
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
	// function checks if current state is a final state								  //
	////////////////////////////////////////////////////////////////////////////////////////
	bool is_final_state(int curr_state) {
		for (int i = 0; i < NUM_OF_FINAL_STATES; i++) {
			if (curr_state == final_states[i])
				return 1;
		}
		return 0;
	}

};