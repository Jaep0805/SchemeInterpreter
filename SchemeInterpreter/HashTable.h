#pragma once
//class to define hash table
#include "HashElement.h"
#define HASHSIZE 300

class HashTable
{
private:
	int capacity; //hashtable total capacity
	int size; //number of elements inside hashtable
	HashElement* list; //a dynamic array of hash elements
public:
	//constructors
	HashTable();
	HashTable(int capacity);
	~HashTable(); //destructor
	void print(); //print function to print all existing hash elements
	int HashFunction(string symbol); //function to calc hashvalue from given symbol
	int insert(string symbol); //function to insert symbol into hashtable
	int search(string symbol); //function to search if symbol exists
	int setlink(int hashval, int linkval); //set link to linkval for corresponding element  with hashvalue 'hashval'
	int getlink(int hashval); //get link of corresponding element with hashvalue 'hashval'
	int getsize();
	string getrealsymbol(int hashvalue); //function to get symbol through following links
	string getsymbol(int hashvalue); //function to get symbol
	int getindexlink(int index); //function to get link of an element at a certain index
	void changelink(int originallink, int newlink); //function to replace all link with new link value
};

