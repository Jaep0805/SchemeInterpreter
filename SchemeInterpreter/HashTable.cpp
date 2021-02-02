#include "HashTable.h"

HashTable::HashTable()
{
	capacity = HASHSIZE;
	size = 0;
	list = new HashElement[capacity]; // create a array of hashelements of size = capacity
}

HashTable::HashTable(int capacity)
{
	this->capacity = capacity;
	size = 0;
	list = new HashElement[capacity];
}

HashTable::~HashTable()
{
	delete[] list; //delete dynamic allocated array
}

void HashTable::print()
{
	//print only nonempty hashelements
	for (int i = 0; i < capacity; i++) {
		if (list[i].getsymbol() != "£Ü0") {
			cout << list[i].gethashvalue() << "  :  " << list[i].getsymbol() << "  :  " << list[i].getlink() << endl;
		}
	}
}

int HashTable::HashFunction(string symbol)
{
	//return hashvalue of symbol
	//hashvalue is calculated using ascii code values
	int sum = 0;
	for (int i = 0; i < symbol.length(); i++) {
		sum += symbol[i];
	}

	return sum % (capacity-1) + 1; //since we want first index 0 to be empty, we % by capacity -1 and add 1
}

int HashTable::insert(string symbol)
{
	if (size == HASHSIZE)throw "Hashtable is full!"; //check is hashtable is full
	int hashvalue = HashFunction(symbol);
	if (search(symbol) != 0) { //check if symbol already exists, if it does, return the index
		return search(symbol);
	}
	for (int i = hashvalue; i < capacity; i++) { //from the hashvalue index, linear probe until empty entry is found
		if (list[hashvalue].gethashvalue() == 0) { //empty entry found
			//insert
			list[hashvalue] = HashElement(hashvalue, symbol, 0);
			size++;
			break;
		}
		else {
			//add 1 to hashvalue (linear probe)
			hashvalue++;
		}
		if (hashvalue == capacity -1) { //if we reached last index of hash table, loop back to 1
			hashvalue = 1;
		}
	}
	return hashvalue;

}

int HashTable::search(string symbol)
{ 
	//search symbol inside hashtable, return index if exists, 0 if it doesnt
	int hashvalue = HashFunction(symbol);
	for (int i = hashvalue; i < capacity; i++) {
		if (list[i].getsymbol() == symbol) {
			return i;
		}
	}
	return 0;
}

int HashTable::setlink(int hashval, int linkval)
{
	list[-1 * hashval].setlink(linkval); //set link of element in index -1 * hashval
	return 1;
}

int HashTable::getsize()
{
	return size;
}

int HashTable::getlink(int hashval)
{
	if (hashval < 0) { //if hashval is less than 0, return 0
		return 0;
	}
	return list[hashval].getlink();
	
}

string HashTable::getrealsymbol(int hashvalue)
{
	//fetch symbol given a hash value
	//print();
	int ind = list[hashvalue].getlink();
	//print();
	if (ind == 0) { //if it has no links just fetch symbol
		return list[hashvalue].getsymbol();
	}
	else return getrealsymbol(-1 * ind); //if it has link, follow the link to fetch pointed value
}

string HashTable::getsymbol(int hashvalue)
{
	return list[hashvalue].getsymbol(); //get symbol pointed by hashvalue
}

int HashTable::getindexlink(int index) //function to get link of an element at a certain index//function to get link of an element at a certain index
{
	return list[index].getlink();
}

void HashTable::changelink(int originallink, int newlink)//function to replace all link with new link value
{
	for (int i = 0; i < capacity; i++) { //search through entire hashtable for corresponding link
		if (list[i].getlink() == originallink) {
			list[i].setlink(newlink); //replace
		}
	}
}
