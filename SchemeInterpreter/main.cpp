#include "SchemeInterpreter.h"

int main() {
	SchemeInterpreter si;
	int result;
	while (true) { //continuously get user command and print results
		string command = si.getCommand(); //get command

		if (command.substr(0,1) != "(") { //if command does not start with "(", return the command's value
			cout << "> Value of " << command << " is " << si.getsymbol(si.getHashValue(command)) << endl;
		}
		else {
			si.initializeTokenizer(command); //tokenize command
			command = si.preprocessing(command); //preprocess the tokenized command
			si.initializeTokenizer(command); //tokenize it again
			//cout << si.getfreelist() << endl;
			if (si.getfreelist()> SIZE - 20) { //collect garbage if mem is almost full
				si.garbageCollection();
				//si.print();
				cout << "> Garbage collection is done" << endl;
			}
			else {
				int root = si.read(si.tokens); //read to store in mem
				result = si.eval(root); //evaluate the mem node tree
				//si.print(); //print mem and hash table
				if (result == -999) { //Error checking for no else statement
					cout << "> Error : no else statement";
				}
				else if (result < 0) { //if command has an evaluated value
					cout << "> Eval result : " << si.getsymbol(-1 * result) << endl;
				}
				else { //otherwise give back the inputted command
					si.printmem(result, true);
				}
			}
		}

	}	
}