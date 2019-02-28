// File containing main funcion

#include <iostream>
using std::cout;
using std::endl;
using std::cerr;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <sstream>
#include "state_machine.cpp"


int main()
{
	// Read each line from the source_code.txt file and store it in a vector of strings  
	string line;
	vector<string> codeArray;
	ifstream sourceCode; 
	sourceCode.open("source_code.txt");
	if (sourceCode.is_open()) {
		while (getline(sourceCode, line)) {
			codeArray.push_back(line);
		}
	}	 
	else
		cerr << "Could not open source_code.txt\n";

	sourceCode.close();



	for (int i = 0; i < codeArray[0].length(); i++ ) {
		//cout << codeArray[0][i] << endl;
	}
	//cout << codeArray.size() << endl;




	// currently testing StateMachine class
	StateMachine FSM;

	cout << FSM.char_to_input(codeArray[0][0]) << " = 0\n";

	const int * final_states = FSM.getFinalStates();

	cout << final_states[3] << " = 8 - symbols\n";

	if (FSM.should_back_up(4))
		cout << "back up\n";
	else
		cout << "DONT back up\n";

	for (int i = 0; i < 9; i++) {
		if (FSM.is_final_state(i))
			cout << i << " is a final state\n";
		else
			cout << i <<" is not a final state\n";
	}
	

	system("pause");

	return 0;
}