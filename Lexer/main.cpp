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


	StateMachine FSM;


	system("pause");

	return 0;
}