#ifndef GAME_H
#define GAME_H
 
#include "globalFunctions.h"
#include "globalVars.h"
#include "Player.h"
#include <vector>

class Game {
private:
	std::string name;
	int numPlayers;
	std::string rules; //no cal?

public:
	Game ( std::string name="");

	std::vector<Player> jugador; //una partida té jugadors

	std::string getName() const;
	void setName(std::string name);
	int getNumPlayers() const;
	void setNumPlayers(int numPlayers);

	void addPlayer(int ID, std::string avatar);
	void removePlayers();
	void showGameInfo();
	void showGameRules();
	void showGameStats();
};
 
#endif
