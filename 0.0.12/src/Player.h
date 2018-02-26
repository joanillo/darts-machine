#ifndef PLAYER_H
#define PLAYER_H
 
#include "globalFunctions.h"
#include "globalVars.h"
 
class Player {
private:
	unsigned int ID;
	std::string avatar;
	std::string pic;
public:
	Player ( int ID=0, std::string avatar="");

	unsigned int getID() const;
	void setID(unsigned int ID);
	std::string getAvatar() const;
	void setAvatar(std::string avatar);
	std::string getAvatarPic() const;
	void setAvatarPic(std::string avatar);
	void showPlayerStats();
};
 
#endif
