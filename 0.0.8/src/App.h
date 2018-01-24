#ifndef APP_H
#define APP_H
 
#include "globalFunctions.h"
#include "globalVars.h"
#include "Screen.h"
#include "Game.h"



class App {
private:
	std::string name;

public:
  	static const int numPantalles= 3; //segurament no cal
	std::vector<Screen> pantalles;
	Game partida; //l'app consisteix en jugar una partida

	App ( std::string name="");

	std::string getName() const;
	void setName(std::string name);

	void loopGame();
	
	void selectGame();
	void selectPlayers();
	void showInfo();

	void play();
	void play_Game(); //genèric

	void updateScreenplay(); //genèric
	void updateScreenplay_301501();
	void updateScreenplay_Count_Up();
	void updateScreenplay_Cricket();
	void updateScreenplay_Round_the_Clock();
	void updateScreenplay_Halve_It();

	void logica(); //genèric
	void logica_301501();
	void logica_Count_Up();
	void logica_Cricket();
	void logica_Round_the_Clock();
	void logica_Halve_It();

	unsigned int getScreenIDfromName(std::string ScreenName) const;
	Screen getScreenfromName(std::string ScreenName) const;
};
 
#endif



