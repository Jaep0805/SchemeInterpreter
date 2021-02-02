#include "HashElement.h"

HashElement::HashElement()
{
	//hashelement basic constructor, all hash values assigned to zero, symbol is assigned to NULL
	this->hashvalue = 0;
	this->symbol = "£Ü0"; //set to NULL
	this->link = 0;
}

HashElement::HashElement(int hashvalue, string symbol, int link)
{
	this->hashvalue = hashvalue;
	this->symbol = symbol;
	this->link = link;
}

int HashElement::gethashvalue()
{
	return hashvalue;
}

string HashElement::getsymbol()
{
	if (symbol == "£Ü0") {
		return "£Ü0";
	}
	return symbol;
}

int HashElement::getlink()
{
	return link;
}

void HashElement::setlink(int linkval)
{
	link = linkval;
}
