/* g++ -Wall -c ScreenElement.cpp */

#include "ScreenElement.h"

// Constructor
ScreenElement::ScreenElement(unsigned int ID, std::string name) {
	this->ID = ID;
	this->name = name;
	this->name2 = "";
	this->clickable = 0;
}

unsigned int ScreenElement::getID() const {
	return ID;
}
 
void ScreenElement::setID(unsigned int ID) {
	this->ID = ID;
}

std::string ScreenElement::getName() const {
	return name;
}
 
void ScreenElement::setName(std::string name) {
	this->name = name;
}

std::string ScreenElement::getName2() const {
	return name2;
}
 
void ScreenElement::setName2(std::string name) {
	this->name2 = name;
}

unsigned int ScreenElement::getClickable() const {
	return clickable;
}
 
void ScreenElement::setClickable(unsigned int clickable) {
	this->clickable = clickable;
}

//unsigned int ScreenElement::getIDFromName(std::string name) const {
//	return 10;
//}
