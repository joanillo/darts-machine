#ifndef SCREENELEMENT_H
#define SCREENELEMENT_H
 
#include "globalFunctions.h"
#include "globalVars.h"
 
class ScreenElement {
private:
	unsigned int ID;
	std::string name;
	std::string name2;
	unsigned int clickable;


public:
	ScreenElement ( unsigned int ID = 0, std::string name="");

	unsigned int getID() const;
	void setID(unsigned int ID);
	std::string getName() const;
	void setName(std::string name);
	std::string getName2() const;
	void setName2(std::string name);
	unsigned int getClickable() const;
	void setClickable(unsigned int clickable);
	//unsigned int getIDFromName(std::string name) const;

};
 
#endif


//id 
//parentname
//name
//text (=name?)
//clickable
//posx
//posy
//font
//fontsize
