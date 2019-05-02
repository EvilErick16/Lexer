# Erick Juarez

Lexical Analyzer using Finite State Machine concept

# 1. Problem Statement:
	
	Syntax analyzer that inplements the assignment expression 

	Input: a vector of token objects that contains tokens and lexemes
		already parsed 

	Output: a file that contains three things; Tokens, Lexemes, and
			Production Rules if grammar is accepted, or Error if grammar
			is not accepted

# 2. How to use the program:

	- Unzip 'syntax_analyzer.zip' and simply run 'Lexer.exe' OR clone github repository and run visual studio solution. 
		
	- Compiled .exe file runs in a wondows 10 environment 
	
	- 'Lexer.exe' file must be in the same folder as the source code file that is being tested. ('test.txt' is included)
		
	- When prompt asks for file name, enter file extension 
		file name: test.txt 		// works
		file name: test 		// does not work 
		
	- Output created and stored in 'output.txt'

# 3. Design of program:

	state_machine.h - declares a finite state macine class that is used to break input file into tokens and lexemes

	syntax.h - defines functions that act as production rules for our grammar then determines if grammar is valid

	main.cpp - main function, gets input file, uses lexer to get tokens and lexemes, 
		   then uses syntax analyzer to determine if grammaris accepted

# 4. Limitations:

	The lexer is written in c++ using Visual Studio 2015; currently
	it does not have support for cross platform.
	Symbols not recognized by the lexer will be treated as whitespace
	and ignored.


# 5. Shortcomings: 

	When grammar is accepted, the output file does not print all 
	the production rules used correctly. 
