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
//unsigned int puntsJugadorRonda[4][2] = {{0,0},{0,0},{0,0},{0,0}}; //Count Up. 2 rondes reduït
//unsigned int puntsJugadorRonda[4][1] = {{0},{0},{0},{0}}; //Count Up. 1 ronda reduït

unsigned int isBusts = false; //301, 501

unsigned int puntsJugadorCricket[4][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};; //Cricket
std::string puntsJugadorCricketSigne[4][7] = {{"","","","","","",""},{"","","","","","",""},{"","","","","","",""},{"","","","","","",""}};; //Cricket
std::string cricketTargets[7] = {"20","19","18","17","16","15","B"}; //Cricket
//unsigned int puntsJugadorCricket[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};; //Cricket reduït
//std::string puntsJugadorCricketSigne[4][3] = {{"","",""},{"","",""},{"","",""},{"","",""}};; //Cricket reduït
//std::string cricketTargets[3] = {"20","19","B"}; //Cricket reduït
unsigned int JugadorCricketClosed[4] = {false, false, false, false};
int isMissCricket = true; //Cricket

unsigned int puntsJugadorRoundTheClock[4] = {0, 0, 0, 0}; //RoundTheClock
std::string roundtheclockTargets[22] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","Bull's Eye", ""}; //RoundTheClock
//std::string roundtheclockTargets[5] = {"1","2","3","Bull's Eye",""}; //RoundTheClock reduït

int puntsJugadorHalveIt[4][7] = {{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1}};; //Halve It. -1: valor inicial; 0: target no aconseguit; 1: target aconseguit
std::string halveitTargets[7] = {"20","4","9","10","8","17","19"}; //Halve It
//int puntsJugadorHalveIt[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};; //Halve It reduït. -1: valor inicial; 0: target no aconseguit; 1: target aconseguit
//std::string halveitTargets[2] = {"20","4"}; //Halve It reduït

//Modify Score
unsigned int puntsJugadorTotalPrev[4] = {0,0,0,0}; //genèric
unsigned int numDartPrev = 1; //genèric
unsigned int tornJugadorPrev = 1; //genèric
unsigned int numRondaPrev = 1; //genèric
int gameOverPrev = false;
int guanyadorPrev = -1; //genèric
unsigned int puntsGuanyadorPrev = 0; //genèric

unsigned int puntsJugadorRoundTheClockPrev[4] = {0, 0, 0, 0}; //RoundTheClock
unsigned int puntsJugadorCricketPrev[4][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};; //Cricket
std::string puntsJugadorCricketSignePrev[4][7] = {{"","","","","","",""},{"","","","","","",""},{"","","","","","",""},{"","","","","","",""}};; //Cricket
//unsigned int puntsJugadorCricketPrev[4][3] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0}};; //Cricket
//std::string puntsJugadorCricketSignePrev[4][3] = {{"","",""},{"","",""},{"","",""},{"","",""}};; //Cricket
unsigned int JugadorCricketClosedPrev[4] = {false, false, false, false}; //Cricket
unsigned int puntsJugadorRondaPrev[4][8] = {{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}}; //Count Up
//unsigned int puntsJugadorRondaPrev[4][2] = {{0,0},{0,0},{0,0},{0,0}}; //Count Up. 2 rondes reduït
//unsigned int puntsJugadorRondaPrev[4][1] = {{0},{0},{0},{0}}; //Count Up. 1 ronda reduït
int puntsJugadorHalveItPrev[4][7] = {{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1}};; //Halve It. -1: valor inicial; 0: target no aconseguit; 1: target aconseguit
//int puntsJugadorHalveItPrev[4][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1}};; //Halve It reduït. -1: valor inicial; 0: target no aconseguit; 1: target aconseguit