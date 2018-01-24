// g++ -Wall -c globalVars.cpp

#include "globalVars.h"
#include <string>

int gameOver = false;
int gameExit = true;
int appInfo = false;
int systemShutdown = false;

unsigned int optionID = 0;
std::string optionName = "";
int idScreen = 0;

unsigned int gameID = 0;
std::string gameName = "";
unsigned int gameNumPlayers = 1;
unsigned int gameAvatarID = 0;
std::string gameAvatar = "";
std::string appInfoOption = "";

//variables que controlen el joc
unsigned int tornJugador = 1; //genèric
unsigned int numRonda = 1; //genèric
std::string strPuntsDiana = ""; //genèric
unsigned int puntsDiana = 0; //genèric
unsigned int isDianaDouble = false;
unsigned int isDianaTreble = false;
unsigned int isSingleBull = false; //genèric
unsigned int isDoubleBull = false; //genèric
int puntsJugadorDart[4][3] = {{-1,-1,-1},{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}}; //genèric
unsigned int puntsJugadorTotal[4] = {0,0,0,0}; //genèric
unsigned int numDart = 1; //genèric
int guanyador = -1; //genèric
unsigned int puntsGuanyador = 0; //genèric
int isMiss = true; //genèric

unsigned int puntsJugadorTotalAntic[4] = {0,0,0,0}; //301...
unsigned int puntsJugadorRonda[4][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}; //Count Up
//unsigned int puntsJugadorRonda[4][2] = {{0,0},{0,0},{0,0},{0,0}}; //Count Up. 2 rondes
//unsigned int puntsJugadorRonda[4][1] = {{0},{0},{0},{0}}; //Count Up. 1 ronda

unsigned int isBusts = false; //301, 501

unsigned int puntsJugadorCricket[4][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};; //Cricket"1","2","3","4","5","6","7"
std::string cricketTargets[7] = {"20","19","18","17","16","15","B"}; //Cricket
unsigned int JugadorCricketClosed[4] = {false, false, false, false};
int isMissCricket = true; //Cricket

unsigned int puntsJugadorRoundTheClock[4] = {0, 0, 0, 0}; //RoundTheClock
std::string roundtheclockTargets[21] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","Bull's Eye"}; //RoundTheClock

int puntsJugadorHalveIt[4][7] = {{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1}};; //Halve It. -1: valor inicial; 0: target no aconseguit; 1: target aconseguit
std::string halveitTargets[7] = {"20","4","9","10","8","17","19"}; //Halve It