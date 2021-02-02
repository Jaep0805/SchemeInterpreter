#include "SchemeInterpreter.h"

SchemeInterpreter::SchemeInterpreter()
{
	//initialize values
	root = NULL;
	first = false;
	//store basic symbols
	leftParenthesisHashValue = ht.insert("(");
	rightParenthesisHashValue = ht.insert(")");
	ht.insert("define");
	ht.insert("+");
	ht.insert("*");
	ht.insert("-");
	ht.insert("/");
	//set index value for token array and unused memnode
	tokenIndex = 0;
	freelist = 1;

	//set up unused mem nodes
	for (int i = 0; i < SIZE; i++) {
		mem[i][0] = i + 1;
		mem[i][2] = i + 1;
	}
	mem[SIZE - 1][0] = 0;
	mem[SIZE - 1][2] = 0;//last node points to 0, signifying end
}

string SchemeInterpreter::getCommand() //function to receive command from user
{
	string command;
	cout << endl;
	cout << "Enter your code" << endl;
	getline(cin, command); //get user input and store into command string
	return command;
}

void SchemeInterpreter::initializeTokenizer(string command)
{
	//reset token array
	for (int i = 0; i < 2 * SIZE; i++) {
		tokens[i] = "";
	}
	tokenIndex = 0;
	//take command string and split into tokens to store in tokens array
	int i = 0; //index to scan char by char
	string temp; //string to store multiple char
	for (char& c : command) {
		//if char is (, immediately add to array and reset temp
		if (c == '(') {
			if (temp.length() > 0) { //if multiple char has been added to temp string, add temp first, and then add )
				tokens[i] = temp;
				i++;
				temp = "";
			}
			tokens[i] = c;
			i++;
			temp = "";
		}
		//if char is ), immediately add to array and reset temp
		else if (c == ')') {
			if (temp.length() > 0) { //if multiple char has been added to temp string, add temp first, and then add )
				tokens[i] = temp;
				i++;
				temp = "";
			}
			tokens[i] = c;
			i++;
			temp = "";
		}
		//if char is ', immediately add to array and reset temp
		else if (c == '\'') {
			if (temp.length() > 0) { //if multiple char has been added to temp string, add temp first, and then add )
				tokens[i] = temp;
				i++;
				temp = "";
			}
			tokens[i] = c;
			i++;
			temp = "";
		}
		//if char is space, check if multiple char has been added to temp string, if so, add temp to array
		else if (c == ' ') { 
			if (temp.length() > 0) {
				tokens[i] = temp;
				i++;
				temp = "";
			}
		}
		//if regular character, turn it into lower case and add to temp string
		else {
			c = tolower(c);
			temp.push_back(c);
		}
	}

}

string SchemeInterpreter::preprocessing(string command)
{
	//preprocessing to take care of non-lambda notation and quotation marks
	string newcommand = "";
	string token;
	while ((token = getNextToken()) != "") {
		//if 'define'comes in, turn it into lambda notation
		if (token == "define") {
			newcommand = newcommand + token; //update command string
			token = getNextToken();
			if (token == "(") {
				token = getNextToken();
				newcommand = newcommand + " " + token + " (lambda ( " + preprocessing(command) + ")"; //recursive call to the inner definition
			}
			else {
				newcommand = newcommand + " " + token; //update command string
			}
		}
		//if quotation mark' comes in, replace it with (quote and add necessary brackets
		else if (token == "\'") {
			newcommand = newcommand + "(quote"; //update command string
			numberleftparen = 0;
			do {
				token = getNextToken();
				newcommand = newcommand + " " + token;//update command string
				if (token == "(") {
					numberleftparen += 1;
				}
				else if (token == ")") {
					numberleftparen -= 1;
				}
			} while (numberleftparen > 0);
			newcommand = newcommand + ")"; //update command string
		}
		else {
			newcommand = newcommand + " " + token;
		}
	}
	return newcommand;
}

string SchemeInterpreter::getNextToken()
{
	return tokens[tokenIndex++]; //return next token in tokens array
}

int SchemeInterpreter::getHashValue(string token)
{
	return ht.insert(token); //call HashTable's hashfunction to get hash value
}


int SchemeInterpreter::read(string* token)
{
	//read from tokens to construct mem node tree and hashtable
	//set index values
	int temp;
	int roots = NULL;
	first = true;
	string next = getNextToken();
	int tokenHashValue = getHashValue(next);
	if (next == "(") {
		next = getNextToken();
		tokenHashValue = getHashValue(next);
		while (next != ")") { //loop until we encounter a closing right parenthesis -> end of function
			//setting right child of mem node
			if (first == true) {
				temp = freelist;
				freelist = mem[freelist][0]; //allocate a mem node from unused mem nodes (freelist)
				roots = temp; 
				first = false;
			}
			else {
				mem[temp][2] = freelist;
				freelist = mem[freelist][0]; //allocate a mem node from unused mem nodes (freelist)
				temp = mem[temp][2];
			}
			//setting left child of mem node
			//if left parenthesis, put it back to tokens by reducing token index, and recursively call the subtree
			if (next == "(") {
				tokenIndex = tokenIndex-1;
				mem[temp][1] = read(token);
				next = getNextToken(); //prepare next token
				tokenHashValue = getHashValue(next);
			}
			//if it is a symbol
			else {
				int htindex = 0;
				htindex = ht.search(next);
				if (htindex == 0) {
					htindex = ht.insert(next);
				}
				mem[temp][1] = -1 * htindex; //set to negative value of hashtable hashvalue
				next = getNextToken(); //prepare next token
				tokenHashValue = getHashValue(next);
			}
			if (first == false) {
				//set right value to 0 for last mem node
				mem[temp][2] = 0;
			}
		}
		root = roots;
		first = false;
		return roots; //return the root of current read call mem node tree
	}

	else return tokenHashValue;

}

void SchemeInterpreter::printmem(int index, bool startList)
{
	//go through mem node tree and print the values in order
	if (root == 0) {
		cout << "()";
	}
	//if start of a tree
	else if (startList == true) {
		cout << "( ";
		printmem(index, false);
		cout << ") ";
	}
	//if the mem's leftchild is negative, it points to a symbol, hence print the symbol
	else if (mem[index][1] < 0) {
		int htindex = -1 * mem[index][1];
		string val = ht.getsymbol(htindex);
		cout << val << " ";
		if (mem[index][2] < 0) {
			htindex = -1 * mem[index][2];
			string val2 = ht.getsymbol(htindex);
			cout << val2 << " ";
		}
		else {
			printmem(mem[index][2], false);
		}
	}
	//if the mem's leftchild is positive, it points to a new subtree, hence recursively call on that tree
	else if (mem[index][1] > 0) {
		printmem(mem[index][1], true);
		if (mem[index][2] > 0) { //if rightchild is also positive, recursively call on that tree as well
			printmem(mem[index][2], false);
		}
	}
	else if (mem[index][2] == 0 && mem[index][1] == 0) { //if both left and rightchild are 0, it means an empty brackets
		cout << "()" << " ";
	}
}

void SchemeInterpreter::print()
{
	if (mem[2][1] == 0 && mem[2][2] == 0) {//if a single character is inputted, print the hashvalue of it
		cout << "Free list's root = " << mem[1][1] << endl;
	}
	else { //print freelist's root value
		cout << "Free list's root = " << freelist << endl;
	}
	cout << "Memory array root = " << root << endl;	//print mem node tree's root value
	cout << endl;
	cout << "Memory table = " << endl;	//print all nodes of memory node tree
	for (int i = 1; i < SIZE; i++) {
		cout << "Index " << i << " : " << mem[i][1] << " | " << mem[i][2] << endl;
	}
	cout << endl;
	cout << "Hash table = " << endl; //print only the nonempty values of hashtable elements
	ht.print();
	cout << endl;
	printmem(root, true);
	cout << endl;
}

int SchemeInterpreter::eval(int node)
{
	if (node == 0) { //if it points to 0 node, return 0
		return node;
	}
	if (node < 0) { //if it points to neg value, return the hashtable link
		int val = ht.getlink(node * -1);
		if (val != 0) {
			return ht.getlink(node * -1);
		}
		else return node;
	}
	int token_index = mem[node][1];
	//int token_index = (mem[node][1] < 0) ? -1 * mem[node][1] : mem[node][1];
	if (token_index == -1 * ht.insert("+")) { //if symbol is +
		int a = eval(mem[mem[node][2]][1]); //get the operand hash value
		string a1 = getsymbol(-1 * a);	//get operand string
		float a2 = stof(a1); //convert to float
		float b = eval(mem[mem[mem[node][2]][2]][1]);
		string b1 = getsymbol(-1 * b);
		float b2 = stof(b1);
		float c = a2 + b2; //add the two float-converted operands
		int val = -1 * getHashValue(to_string(c)); //insert to hashtable and return the hashval
		return val;
	}
	else if (token_index == -1 * ht.insert("-")){ //if symbol is -, similar to +
		int a = eval(mem[mem[node][2]][1]);
		string a1 = getsymbol(-1 * a);
		int a2 = stoi(a1);
		int b = eval(mem[mem[mem[node][2]][2]][1]);
		string b1 = getsymbol(-1 * b);
		int b2 = stoi(b1);
		int c = a2 - b2;
		int val = -1 * getHashValue(to_string(c));
		return val;
	}
	else if (token_index == -1 * ht.insert("*")) {//if symbol is *, similar to +
		int a = eval(mem[mem[node][2]][1]);
		string a1 = getsymbol(-1 * a);
		int a2 = stoi(a1);
		int b = eval(mem[mem[mem[node][2]][2]][1]);
		string b1 = getsymbol(-1 * b);
		int b2 = stoi(b1);
		int c = a2 * b2;
		int val = -1 * getHashValue(to_string(c));
		return val;
	}
	else if (token_index == -1 * ht.insert("=")) { //if symbol is "=",  compare equality between number
		//check if not a number
		if (!isNumber(mem[mem[node][2]][1]) || !isNumber(mem[mem[mem[node][2]][2]][1])) {
			return -1 * getHashValue("#f"); //if not a number, return false
		}
		else {
			//get symbol in string and convert to float
			string strarg1 = ht.getrealsymbol(-1 * mem[mem[node][2]][1]);
			string strarg2 = ht.getrealsymbol(-1 * mem[mem[mem[node][2]][2]][1]);
			int arg1 = stof(strarg1);
			int arg2 = stof(strarg2);
			if (arg1 == arg2) { //compare float values and return true if equal
				return -1 * getHashValue("#t");
			}
			else {
				return -1 * getHashValue("#f");
			}
		}
	}
	else if(token_index == -1 * ht.insert("eq?")) { //if symbol is "eq?", compare memory location
		if (eval(mem[mem[node][2]][1]) == eval(mem[mem[mem[node][2]][2]][1])) {
			return -1 * getHashValue("#t");
		}
		else {
			return -1 * getHashValue("#f");
		}
	}
	else if (token_index == -1 * ht.insert("equal?")) { //if symbol is "equal?", equality check for list, values
		if (checkStructure(eval(mem[mem[node][2]][1]), eval(mem[mem[mem[node][2]][2]][1]))) { //use check structure function to check mem subtree structure and values
			return -1 * getHashValue("#t");
		}
		else {
			return -1 * getHashValue("#f");
		}
	}
	else if (token_index == -1 * ht.insert("number?")) { //if symbol is "number?", call isNumber function to check if the next token is a number
		int val = eval(mem[mem[node][2]][1]);
		if (isNumber(val)) {
			return -1 * getHashValue("#t");
		}
		else { 
			return -1 * getHashValue("#f");
		}
	}
	else if (token_index == -1 * ht.insert("symbol?")) {//if symbol is "symbol?", call isNumber function to check if the next token is a number
		int result = eval(mem[mem[node][2]][1]);
		if(isNumber(result) == 0) { //not number = symbol
			return -1 *getHashValue("#t");
		}
		else {
			return -1 * getHashValue("#f");
		}
	}
	else if (token_index == -1 * ht.insert("null?")) { //if symbol is "null?",  check if following token or subtree evaluates to 0
		if (mem[node][2] == 0 || eval(mem[node][2]) == 0) {
			return -1 * getHashValue("#t");
		}
		else {
			return -1 * getHashValue("#f");
		}
	}
	else if (token_index == -1 * ht.insert("cons")) { //if symbol is "cons?", evaluate the next two subtrees, allocate new mem node, and set the mem node's left and right child to each evaluation
		int temp = freelist;
		freelist = mem[freelist][0]; //allocate a mem node from unused mem nodes (freelist)
		mem[temp][1] = eval(mem[mem[node][2]][1]); //set left child
		mem[temp][2] = eval(mem[mem[mem[node][2]][2]][1]);//set right child
		return temp;
	}
	else if (token_index == -1 * ht.insert("cond")) { //if symbol is "cond", recursively check for statements, an evaluate them
		while (mem[mem[node][2]][2] != 0) {
			node = mem[node][2];
			if (eval(mem[mem[node][1]][1]) == -1 * ht.insert("#t")) { //check if condition evaluates to true 
				return eval(mem[mem[mem[node][1]][2]][1]);  //if it does, return the value
			}
		}
		if (mem[mem[mem[node][2]][1]][1] != -1 * ht.insert("else")) { //if there is no else
			return -999;
		}
		else {
			return eval(mem[mem[mem[mem[node][2]][1]][2]][1]); //recursive call to next conditions
		}
	}
	else if (token_index == -1 * ht.insert("car")) { //if symbol is 'car', return the first val of a subtree evaluation
		return mem[eval(mem[mem[node][2]][1])][1]; //evaluate location and return the first left child
	}
	else if (token_index == -1 * ht.insert("cdr")) { //if symbol is 'car', return subtree evaluation excluding first val
		return mem[eval(mem[mem[node][2]][1])][2]; //evaluate location and return the  right node, excluding first left child
	}
	else if (token_index == -1 * ht.insert("define")) { //define to create functions or set variables
		if (mem[mem[mem[node][2]][2]][1] < 0) { //if the operand is just a number
			ht.setlink(mem[mem[node][2]][1], mem[mem[mem[node][2]][2]][1]);
		}
		else { //if the second operand is a function
			ht.setlink(mem[mem[node][2]][1], (mem[mem[mem[node][2]][2]][1]));
		}		
		return node;
	}
	else if (token_index == -1 * ht.insert("quote")) { //if symbol is a quotation mark, return next token(right subtree)
		return mem[mem[node][2]][1];
	}
	else if(ht.getlink(-1 * token_index) > 0){ //else if link is a positive value, it means its a udf
		int link = ht.getlink(-1 * token_index); //get the mem node location of udf function we are calling
		//variables to store prev values and result
		int result; 
		int arg[3];
		//print();
		if (mem[mem[node][2]][2] == 0) { //for single argument
			string s;
			if (mem[mem[node][2]][1] < 0) { //if it points to a value
				s = ht.getrealsymbol(-1 * mem[mem[node][2]][1]);
			}
			else { //if it points to a subtree
				int g = eval(mem[mem[node][2]][1]);
				s = to_string(g);
			}

			//saveparam[0] = ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]);
			push(ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]), 0, 0); //save arg value to stack
			arg[0] = mem[mem[mem[link][2]][1]][1];
			//ht.print();

			if (mem[mem[node][2]][1] < 0) {//if it points to a value
				ht.setlink(arg[0], -1 * getHashValue(s));
			}
			else {//if it points to a subtree
				ht.setlink(arg[0], stoi(s));
			}			
			//ht.print();
			result = eval(mem[mem[mem[link][2]][2]][1]); //evaluate udf using given argument
			pindex--;
			ht.setlink(mem[mem[mem[link][2]][1]][1], pop0()); //pop from saved stack and load the saved value on to arg
		}
		else if (mem[mem[mem[node][2]][2]][2] == 0) { //for two arguments
			string s;
			if (mem[mem[node][2]][1] < 0) {
				s = ht.getrealsymbol(-1 * mem[mem[node][2]][1]);
			}
			else {
				int g = eval(mem[mem[node][2]][1]);
				s = to_string(g);
			}
			string s2;
			if (mem[mem[node][2]][1] < 0) {
				s2 = ht.getrealsymbol(-1 * mem[mem[mem[node][2]][2]][1]);
			}
			else {
				int g2 = eval(mem[mem[mem[node][2]][2]][1]);
				s2 = to_string(g2);
			}

			push(ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]), ht.getlink(-1 * mem[mem[mem[mem[link][2]][1]][2]][1]), 0);

			//saveparam[0] = ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]);
			arg[0] = mem[mem[mem[link][2]][1]][1];
			ht.setlink(arg[0], -1 * getHashValue(s));

			// saveparam[1] = ht.getlink(-1 * mem[mem[mem[mem[link][2]][1]][2]][1]);
			arg[1] = mem[mem[mem[mem[link][2]][1]][2]][1];
			ht.setlink(arg[1], -1 * getHashValue(s2));


			result = eval(mem[mem[mem[link][2]][2]][1]);

			pindex--;
			ht.setlink(mem[mem[mem[link][2]][1]][1], pop0());
			ht.setlink(mem[mem[mem[mem[link][2]][1]][2]][1], pop1());
		}
		else if (mem[mem[mem[node][2]][2]][2] != 0) { //for three arguments
			string s;
			if (mem[mem[node][2]][1] < 0) {
				s = ht.getrealsymbol(-1 * mem[mem[node][2]][1]);
			}
			else {
				int g = eval(mem[mem[node][2]][1]);
				s = to_string(g);
			}
			string s2;
			if (mem[mem[node][2]][1] < 0) {
				s2 = ht.getrealsymbol(-1 * mem[mem[mem[node][2]][2]][1]);
			}
			else {
				int g2 = eval(mem[mem[mem[node][2]][2]][1]);
				s2 = to_string(g2);
			}
			string s3;
			if (mem[mem[node][2]][1] < 0) {
				s3 = ht.getrealsymbol(-1 * mem[mem[mem[mem[node][2]][2]][2]][1]);
			}
			else {
				int g3 = eval(mem[mem[mem[mem[node][2]][2]][2]][1]);
				s3 = to_string(g3);
			}

			push(ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]), ht.getlink(-1 * mem[mem[mem[mem[link][2]][1]][2]][1]), ht.getlink(-1 * mem[mem[mem[mem[mem[link][2]][1]][2]][2]][1]));

			//saveparam[0] = ht.getlink(-1 * mem[mem[mem[link][2]][1]][1]);
			arg[0] = mem[mem[mem[link][2]][1]][1];
			ht.setlink(arg[0], -1 * getHashValue(s));
			
			//saveparam[1] = ht.getlink(-1 * mem[mem[mem[mem[link][2]][1]][2]][1]);
			arg[1] = mem[mem[mem[mem[link][2]][1]][2]][1];
			ht.setlink(arg[1], -1 * getHashValue(s2));
		
			//saveparam[2] = ht.getlink(-1 * mem[mem[mem[mem[mem[link][2]][1]][2]][2]][1]);
			arg[2]= mem[mem[mem[mem[mem[link][2]][1]][2]][2]][1];
			ht.setlink(arg[2], -1 * getHashValue(s3));

			result = eval(mem[mem[mem[link][2]][2]][1]);

			pindex--;
			ht.setlink(mem[mem[mem[link][2]][1]][1], pop0());
			ht.setlink(mem[mem[mem[mem[link][2]][1]][2]][1], pop1());
			ht.setlink(mem[mem[mem[mem[mem[link][2]][1]][2]][2]][1], pop2());
		}
		return result;
	}
	//else return evaluation of token_index
		return eval(token_index);	
}

int SchemeInterpreter::eval() //driver function
{
	return eval(root);
}

int SchemeInterpreter::isNumber(int hashval) //check if hashval correspond to a number
{
	string val = ht.getrealsymbol(-1 * hashval); //get symbol of hashval from hashtable
	return val.find_first_not_of("0123456789-") == string::npos; //if it contains any of the values not in quotation mark, it is not anumber
}

string SchemeInterpreter::getsymbol(int hashvalue)
{
	return ht.getrealsymbol(hashvalue); //get real symbol follows link in hashtables to get the value that the symbol is pointing to
}

bool SchemeInterpreter::checkStructure(int arg1, int arg2) //recursively call checkstructure on subtrees to compare arg1 and arg2 tree
{
	bool val = true;
	//inorder traversal to check structure
	if (arg1 <= 0 || arg2 <= 0) {
		if (arg1 != arg2) {
			return false;
		};
	}
	else {
		if (checkStructure(mem[arg1][1], mem[arg2][1])) { //left child
			val = true;
		}
		else {
			return false;
		};
		if (checkStructure(mem[arg1][2], mem[arg2][2])) { //right child
			val = true;
		}
		else {
			return false;
		};
	}
	return val;
}
 
int SchemeInterpreter::lastmemnode(int node) //recursively call until last mem node in a mem subtree is reached
{	
	if (node > lastmem) {
		lastmem = node;
	}
	if (mem[node][1] > 0) { //recursive call on left child if positive
		lastmemnode(mem[node][1]);
	}
	if (mem[node][2] > 0) { //recursive call on right child if positive
		lastmemnode(mem[node][2]);
	}
	return lastmem;
}

int SchemeInterpreter::getfreelist()
{
	return freelist;
}

//pop functions for pstack parameter 0, 1, 2
int SchemeInterpreter::pop0()
{
	return pstack[pindex][0];
}
int SchemeInterpreter::pop1()
{
	return pstack[pindex][1];
}
int SchemeInterpreter::pop2()
{
	return pstack[pindex][2];
}

void SchemeInterpreter::push(int val1, int val2, int val3) //push function saving parameters onto pstack
{
	pstack[pindex][0] = val1; //param0
	pstack[pindex][1] = val2; //param1
	pstack[pindex][2] = val3; //param2
	pindex++;
}


int SchemeInterpreter::garbageCollection() //garbage collection
{
	int link[SIZE] = {};
	int counter = 0;
	//find all hash table elements with positive links 
	for (int i = 0; i < HASHSIZE; i++) {
		if (ht.getindexlink(i) > 0) {
			link[counter] = ht.getindexlink(i);
			counter += 1;
		}
	}
	//sort the link array in ascending order
	int key, j;
	for (int i = 1; i < size(link); i++) { //insertion sort
		key = link[i];
		j = i;
		while (j > 0 && link[j - 1] > key) {
			link[j] = link[j - 1];
			j--;
		}
		link[j] = key;   
	}

	//for each subtree pointed by positive link, update its position on memtree and update the positive link pointer
	int counter2 = 1;
	for(int i = 0; i < SIZE; i++) {
		int currlink = link[i];
		if (currlink != 0) {
			ht.changelink(currlink, counter2); //update positive link pointer
			lastmem = 0;
			int val = lastmemnode(currlink);
			for (int j = currlink; j <= val; j++) { //update mem tree
				mem[counter2][0] = mem[j][0]; 
				mem[counter2][1] = mem[j][1];
				mem[counter2][2] = mem[j][2];
				for (int k = 0; k < 3; k++) {
					if(mem[counter2][0] > 0) {
						mem[counter2][0] = mem[j][0] - (j - counter2);
					}
					if(mem[counter2][1] > 0) {
						mem[counter2][1] = mem[j][1] - (j - counter2);
					}
					if (mem[counter2][2] > 0) {
						mem[counter2][2] = mem[j][2] - (j - counter2);
					}
				}
				counter2++;
			}
		}	
	}
	freelist = counter2; //set the freelist index to last counter2 value

	//after all pointed mem subtrees are set, delete the rest of memtree
	while (counter2 < SIZE) {
		mem[counter2][0] = counter2 + 1;
		mem[counter2][1] = 0;
		mem[counter2][2] = counter2 + 1;
		counter2++;
	}
	return 0;
}

