#ifndef GLOBALVARS_H
#define GLOBALVARS_H

#include <string>

extern int gameOver;
extern int gameExit;
extern int appInfo;
extern int systemShutdown;

extern unsigned int optionID;
extern std::string optionName;
extern int idScreen;

extern unsigned int gameID; //de moment no el faig servir?
extern std::string gameName; //de moment no el faig servir?
extern unsigned int gameNumPlayers; //de moment no el faig servir?
extern unsigned int gameAvatarID; //de moment no el faig servir?
extern std::string gameAvatar; //de moment no el faig servir?
extern std::string appInfoOption;

//variables que controlen el joc
extern unsigned int tornJugador; //genèric
extern unsigned int numRonda; //genèric
extern std::string strPuntsDiana; //genèric
extern unsigned int puntsDiana; //genèric
extern unsigned int isDianaDouble; //genèric
extern unsigned int isDianaTreble; //genèric
extern unsigned int isSingleBull; //genèric
extern unsigned int isDoubleBull; //genèric
extern int puntsJugadorDart[4][3]; //genèric
extern unsigned int puntsJugadorTotal[4]; //genèric
extern unsigned int numDart; //genèric
extern int guanyador; //genèric
extern unsigned int puntsGuanyador; //genèric
extern int isMiss; //genèric

extern unsigned int puntsJugadorTotalAntic[4]; //301...
extern unsigned int puntsJugadorRonda[4][8]; //Count Up
//extern unsigned int puntsJugadorRonda[4][2]; //Count Up, 2 rondes
//extern unsigned int puntsJugadorRonda[4][1]; //Count Up, 1 ronda
extern unsigned int isBusts; //301, 501
extern unsigned int puntsJugadorCricket[4][7]; //Cricket
extern std::string cricketTargets[7]; //Cricket
extern unsigned int JugadorCricketClosed[4]; //Cricket
extern int isMissCricket; //Cricket
extern unsigned int puntsJugadorRoundTheClock[4]; //RoundTheClock
extern std::string roundtheclockTargets[21]; //RoundTheClock
extern int puntsJugadorHalveIt[4][7]; //Halve It
extern std::string halveitTargets[7]; //Halve It

#endif /* GLOBALVARS_H */

