/* g++ -Wall -c Player.cpp */

#include "Player.h"

// Constructor
Player::Player(int ID, std::string avatar) {
	this->ID = ID;
	this->avatar = avatar;
	this->pic = replace(avatar," ","_") + ".png";
}

unsigned int Player::getID() const {
	return ID;
}

void Player::setID(unsigned int ID) {
	this->ID = ID;
}


std::string Player::getAvatar() const {
	return avatar;
}

std::string Player::getAvatarPic() const {
	return pic;
}

void Player::setAvatar(std::string avatar) {
	this->avatar = avatar;
}

void Player::setAvatarPic(std::string avatar) {
	this->pic = replace(avatar," ","_") + ".png";
}

void Player::showPlayerStats() {
	std::cout << "*** PLAYER STATS " << this->avatar << " *************" << std::endl;
	std::fstream myfile;
	std::string line;

	myfile.open (("../resources/playerstats/" + replace(this->avatar," ","_") + ".txt").c_str());

	if (myfile.is_open())
	{
	while ( getline (myfile,line) )
	{
		std::cout << line << std::endl;
	}
	myfile.close();
	}
	else 	std::cout << "Unable to open file" << std::endl;
}
