#ifndef SCREEN_H
#define SCREEN_H
 
#include "globalFunctions.h"
#include "globalVars.h"
#include "ScreenElement.h"
 
class Screen {
private:
	unsigned int ID;
	std::string name;
	int numElements; //?


public:
	Screen ( unsigned int ID = 0, std::string name = "", int numElements = 1);

	std::vector<ScreenElement> pantallaelements;

	unsigned int getID() const;
	void setID(unsigned int ID);
	std::string getName() const;
	void setName(std::string name);

	void createScreenElements(std::string screenName);
	unsigned int getScreenElementIDfromName(std::string elementName) const;
	ScreenElement getScreenElementfromName(std::string elementName) const;
};
 
#endif
