#pragma once
//hash element class: defines the elements the hash table is composed of
#include "ref.h"


using namespace std;

class HashElement
{
private:
	int hashvalue; //hash value to index the symbols
	string symbol; //symbols corresonding to the hash value
	int link; //pointer to mem node or other hash element
public:
	//constructors
	HashElement();
	HashElement(int hashvalue, string symbol, int link);
	//getters to obtain private variables
	int gethashvalue();
	string getsymbol();
	//setters and getters for link
	int getlink();
	void setlink(int linkval);
};

