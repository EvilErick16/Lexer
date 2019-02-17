// File containing main funcion

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <sstream>


int main()
{
	// Read each line from the source_code file 
	string line;
	vector<string> codeArray;
	ifstream sourceCode;			
	sourceCode.open("source_code.txt");

	if (sourceCode.is_open()) {
		while (getline(sourceCode, line)) {
			codeArray.push_back(line);
			cout << codeArray.back() << endl;
		}
	}

	sourceCode.close();

	system("pause");


	return 0;
}