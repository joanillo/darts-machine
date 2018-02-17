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
//extern unsigned int puntsJugadorRonda[4][2]; //Count Up, 2 rondes reduït
//extern unsigned int puntsJugadorRonda[4][1]; //Count Up, 1 ronda reduït
extern unsigned int isBusts; //301, 501

extern unsigned int puntsJugadorCricket[4][7]; //Cricket
extern std::string puntsJugadorCricketSigne[4][7]; //Cricket
extern std::string cricketTargets[7]; //Cricket
//extern unsigned int puntsJugadorCricket[4][3]; //Cricket reduït
//extern std::string puntsJugadorCricketSigne[4][3]; //Cricket reduït
//extern std::string cricketTargets[3]; //Cricket reduït
extern unsigned int JugadorCricketClosed[4]; //Cricket
extern int isMissCricket; //Cricket

extern unsigned int puntsJugadorRoundTheClock[4]; //RoundTheClock
extern std::string roundtheclockTargets[22]; //RoundTheClock
//extern std::string roundtheclockTargets[5]; //RoundTheClock reduït
extern int puntsJugadorHalveIt[4][7]; //Halve It
extern std::string halveitTargets[7]; //Halve It

//Modify Score
extern unsigned int puntsJugadorTotalPrev[4]; //genèric
extern unsigned int numDartPrev; //genèric
extern unsigned int tornJugadorPrev; //genèric
extern unsigned int numRondaPrev; //genèric
extern int gameOverPrev;
extern int guanyadorPrev; //genèric
extern unsigned int puntsJugadorRoundTheClockPrev[4]; //RoundTheClock

extern unsigned int puntsJugadorCricketPrev[4][7]; //Cricket
extern std::string puntsJugadorCricketSignePrev[4][7]; //Cricket
//extern unsigned int puntsJugadorCricketPrev[4][3]; //Cricket reduït
//extern std::string puntsJugadorCricketSignePrev[4][3]; //Cricket reduït
extern unsigned int JugadorCricketClosedPrev[4]; //Cricket

extern unsigned int puntsGuanyadorPrev; //genèric

#endif /* GLOBALVARS_H */

