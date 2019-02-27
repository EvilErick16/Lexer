//This file defines the State Machine class 

// used to read from the transition_table.txt file 
#include <fstream>
using std::ifstream;
#include <iostream>
using std::cerr;
#include <string>
using std::string;
#include <vector>
using std::vector;


// create enumerated data types to access the array containing the Sate Trasition table 
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


//State machine class definition 
class StateMachine {


private:

	//private variables for the State Machine class 
	int curr_state;		
	int new_state;
	const State final_states[4] = { END_OF_IDENTIFIER, END_OF_NUMBER, END_OF_COMMENT, SYMBOLS };
	int state_transition_table[9][8];


public:
	// StateMacine constructor initializes the State_transition_table
	StateMachine() {

		// open text file to get values inside State transition table
		vector<string> vals;
		ifstream table;
		string value;
		table.open("transition_table.txt");
		if (table.is_open()) {
			while (getline(table, value)) {
				vals.push_back(value);
				//std::cout << value << std::endl;
			}
		}
		else
			cerr << "Could not open transition_table.txt\n";

		for (int state = 0, vect_val = 0; state < 9; state++) {
			for (int input = 0; input < 8; input++) {
				state_transition_table[state][input] = std::stoi(vals[vect_val]);
				vect_val++;
				std::cout << state_transition_table[state][input] << "  ";
			}
			std::cout << "\n";
		}

		
	}
	// function returns a const pointer to the first element of the final states array;
	const State * getFinalStates() {
		const State * ptr = final_states;
		return ptr;
	};


};