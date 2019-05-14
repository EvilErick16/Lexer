// File containing main funcion

#include <algorithm>
using std::all_of;

#include "state_machine.h"
#include "syntax.h"


int main()
{
	 
	//////////////////////////////////////////////////////////////////////////////////////////////////
	//	Read the source code file into a buffer 
	//////////////////////////////////////////////////////////////////////////////////////////////////
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
	else {
		cerr << "Could not open " << file_name << endl;
		return -1;
	}

	sourceCode.close();

	///////////////////////////////////////////////////////////////////////////////////////////////
	//	Iterate through the buffer looking for Tokens and their corresponding lexemes 
	///////////////////////////////////////////////////////////////////////////////////////////////
	vector<tokens> token_lexeme;
	StateMachine FSM;
	int curr_state = 0;
	int lexeme_start = 0;

	/*cout << "--------------------------------" << endl;
	cout << left << setw(25) << "TOKENS" << "Lexemes" << endl;
	cout << "--------------------------------" << endl << endl;*/

	// Iterate through every character in the codeArray vector 
	for (int line = 0; line < codeArray.size(); line++) {
		for (int char_ = 0; char_ <= codeArray[line].length(); char_++) {

			// If this char is the begining of a token, set the variable
			if (curr_state == 0) {
				lexeme_start = char_;
			}

			// Get the next input, transform the char into an int 
			int curr_input = FSM.char_to_input(codeArray[line][char_]);

			// Find the next state based on the current state and current input
			curr_state = FSM.check_input(curr_state, curr_input);

			// Check if the new state is a final state for a token 
			if (FSM.is_final_state(curr_state)) {
				
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
					if (FSM.getTokenName(curr_state, lex) != "OTHER") {
						// Add the found token and lexeme to their respective vectors
						token_lexeme.push_back(tokens(FSM.getTokenName(curr_state, lex), lex));
						//cout << left << setw(15) << token_lexeme.back().token <<
							//setw(10) << " " << token_lexeme.back().lexeme << endl;
					}
				}				

				// Set the current state to begining state
				curr_state = 0;
				}
			}
		}

	///////////////////////////////////////////////////////////////////////////////////////////////
	// Output Tokens and Lexemes to a file
	///////////////////////////////////////////////////////////////////////////////////////////////
	ofstream out("output.txt");
	//out << "--------------------------------" << endl;
	//out << left << setw(15) << "TOKENS" << setw(10) << " " << "Lexemes" << endl;
	//out << "--------------------------------" << endl << endl;
	//for (int i = 0; i < token_lexeme.size(); i++)
	//{
	//	out << left << setw(15) << token_lexeme[i].token <<
	//		setw(10) << " " << token_lexeme[i].lexeme << endl;
	//	cout << left << setw(15) << token_lexeme[i].token <<
	//		setw(10) << " " << token_lexeme[i].lexeme << endl;
	//}

	// Check if the syntax is accepted
	if (!analyze_syntax(token_lexeme, out)) {
		cout << "Syntax error" << endl;
		out << "ERROR: syntax error found in the source code" << endl;
	}

	// Close output file 
	out.close();

	// End main 
	return 0;
}