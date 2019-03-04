// File containing main funcion

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
using std::cin;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <algorithm>
using std::all_of;

#include <sstream>
#include "state_machine.cpp"


int main()
{
	 
	string line;
	vector<string> codeArray;
	ifstream sourceCode; 
	string file_name;

	//get the name of the file from the user 
	cout << "Filename: ";
	getline(cin,file_name);

	// Read each line from the source code file and store it in a vector of strings 
	sourceCode.open(file_name);
	if (sourceCode.is_open()) {
		while (getline(sourceCode, line)) {
			codeArray.push_back(line);
		}
	}	 
	else
		cerr << "Could not open " << file_name << endl;

	sourceCode.close();

	vector<string> lexemes;
	vector<string> tokens;
	StateMachine FSM;
	int curr_state = 0;
	int lexeme_start = 0;

	// For every character in the vector 
	for (int line = 0; line < codeArray.size(); line++) {
		for (int char_ = 0; char_ <= codeArray[line].length(); char_++) {

			// If this is the begining of a token, set the variable
			if (curr_state == 0) {
				lexeme_start = char_;
			}

			// Get the next input 
			int curr_input = FSM.char_to_input(codeArray[line][char_]);

			// Find the next state 
			curr_state = FSM.check_input(curr_state, curr_input);

			// If this is a final state for a token 
			if (FSM.is_final_state(curr_state)) {

				//cout << " Final State: " << curr_state << endl;
				
				// If the state machine shold back up, back up 
				if (FSM.should_back_up(curr_state)) {
					char_--;
				}

				// If current state is the end of a comment, ignore it
				if (curr_state != 7) {
					string lex = "";
					for (int i = lexeme_start; i <= char_; i++) {
						lex += codeArray[line][i];
					}
					// If the token is a whitespace, ignore it 
					if (FSM.getTokenName(curr_state, lex) != "SPACE") {
						tokens.push_back(FSM.getTokenName(curr_state, lex));
						lexemes.push_back(lex);
						cout << tokens.back() << "			" << lexemes.back() << endl;
					}
				}
				
				

				// Set the current state to begining state
				curr_state = 0;
				}
			}
		}

	system("pause");

	return 0;
}