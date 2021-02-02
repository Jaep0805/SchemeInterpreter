#pragma once
//Interpreter class to read user input, create and manage mem node tree and print results
#include "HashTable.h"
#define SIZE 100
using namespace std;

class SchemeInterpreter
{
public : 
	int root; //index of memory's root node
	bool first; //bool value to keep track of first root commands
	int tokenIndex; //index of tokens array
	//store both left and right parenthesis hash value
	int leftParenthesisHashValue; 
	int rightParenthesisHashValue;
	//# of ( and )
	int numberleftparen; 
	int numberrightparen;
	string tokens[2*SIZE]; //array of strings to store tokenized command
	HashTable ht; 
	int mem[SIZE][3];  //2d array to store SIZEx3 elements, where the 2nd index correspond to next, left, right
	int freelist; //index of unused memory nodes
	int lastmem = 0; //index for garbage collection
	int pstack[20][3]; //stack to store values for recursive call, support up to 3 parameter
	int pindex = 0; //index for recursive call stack



public:
	SchemeInterpreter(); //constructor
	string getCommand(); //function to get user input command
	void initializeTokenizer(string command); //take input command and tokenize it into array of strings
	string preprocessing(string command);
	string getNextToken(); //get next string in tokens array
	int getHashValue(string token); //get hashvalue of string token
	int read(string* token); //read all tokens and construct mem node tree and insert into hash table
	void printmem(int index, bool startList); //print the mem node tree values in order and empty it
	void print(); //print all data constructed from user input
	int eval(int root); //eval to calc result of command
	int eval(); //driver function
	int isNumber(int hashval); //check if number
	string getsymbol(int hashvalue); //get symbol from hashtable
	bool checkStructure(int arg1, int arg2);
	int garbageCollection(); //function to execute garbage collection
	int lastmemnode(int node); //function to find last node of mem subtree
	int getfreelist(); //getter for freelist
	void push(int val1, int val2, int val3); //pstack push
	//pop function for pstack
	int pop0(); 
	int pop1();
	int pop2();

};

