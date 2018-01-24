/* g++ -Wall -std=c++11 -c Game.cpp */
/* g++ -Wall -std=c++0x -c Game.cpp */

#include "Game.h"

// Constructor
Game::Game(std::string name) {
	this->name = name;
}

std::string Game::getName() const {
	return name;
}
 
void Game::setName(std::string name) {
	this->name = name;
}

int Game::getNumPlayers() const { //NO CAL, és un vector, puc fer size()
	return numPlayers;
	//return this->jugador.size();
}

void Game::setNumPlayers(int numPlayers) { //NO CAL, és un vector, puc fer size()
	this->numPlayers = numPlayers;
}

void Game::addPlayer(int ID, std::string avatar) {
    Player jugadorTemp(ID, avatar);
	this->jugador.push_back(jugadorTemp);
}

void Game::removePlayers() {
	this->jugador.clear();
}

void Game::showGameInfo() {
	std::cout << "*** INFO *************" << std::endl;
	//nom de la partida
	std::cout << this->name << std::endl;
	//nom dels jugadors
    for(Player pl : jugador) {
		std::cout << "\t" << pl.getAvatar() << std::endl;
    }
    std::cout << std::endl;
}

void Game::showGameRules() {
	std::cout << "*** REGLES " << this->name << " *************" << std::endl;
	std::fstream myfile;
	std::string line;

	myfile.open ("../resources/rules/" + replace(this->name," ","_") + ".txt");

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

void Game::showGameStats() {
	std::cout << "*** GAME STATS " << this->name << " *************" << std::endl;
	std::fstream myfile;
	std::string line;

	myfile.open ("../resources/gamestats/" + replace(this->name," ","_") + ".txt");

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
