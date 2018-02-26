/* g++ -Wall -std=c++0x -c App.cpp */

#include "App.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//per tal de què compili amb HideStdinKeystrokes():
#include <termios.h>
#include <sys/ioctl.h>


void cleanVariables();
int updateVarsScore();
std::string completar_nearest(std::string, std::string, std::string);

//http://stackoverflow.com/questions/13694170/how-do-i-hide-user-input-with-cin-in-c
void HideStdinKeystrokes()
{
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    /* we want to disable echo */
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void ShowStdinKeystrokes()
{
   termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    /* we want to reenable echo */
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void enable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

// Constructor
App::App(std::string name) {
    unsigned int id = 0;
	this->name = name;

    //construim les pantalles
    Screen pantallaTemp;

    pantallaTemp.setID(id);
    pantallaTemp.setName("new_game");
    pantallaTemp.createScreenElements(pantallaTemp.getName());
    pantalles.push_back(pantallaTemp);
    id++;

    pantallaTemp.setID(id);
    pantallaTemp.setName("num_players");
    pantallaTemp.createScreenElements(pantallaTemp.getName());
    pantalles.push_back(pantallaTemp);
    id++;

    pantallaTemp.setID(id);
    pantallaTemp.setName("assign_avatars");
    pantallaTemp.createScreenElements(pantallaTemp.getName());
    pantalles.push_back(pantallaTemp);
    id++;

    pantallaTemp.setID(id);
    pantallaTemp.setName("show_app_info");
    pantallaTemp.createScreenElements(pantallaTemp.getName());
    pantalles.push_back(pantallaTemp);
    id++;

}

std::string App::getName() const {
	return name;
}
 
void App::setName(std::string name) {
	this->name = name;
}

/**************************
* Bucle principal del joc *
**************************/
void App::loopGame()  {
	//HideStdinKeystrokes(); //amaguem el echo de l'entrada del teclat
    enable_raw_mode();

    //bucle principal del jocs/sessió. Una sessió és una successió de partides, fins que ens cansem i fem shutdown
    while (systemShutdown == false) {

        if (gameExit==true) {
            App::selectGame();
        } else if (appInfo==true) {
            App::showInfo();
        } else if (gameExit==false) {
            selectPlayers();
            //mostrar INFO del que hem escollit
            partida.showGameInfo();
            //ja podem jugar
            //std::cout << partida.getID() << "CCC " << partida.getName() << std::endl;
            App::play();
        }

    } //fi bucle principal
    //ShowStdinKeystrokes();
    disable_raw_mode();
    tcflush(0, TCIFLUSH); // Clear stdin to prevent characters appearing on prompt 
}

void App::selectGame() {
    gameExit = false;
    appInfo = false;

    //seleccionem el joc
    idScreen = App::getScreenIDfromName("new_game");
    std::cout << "***************************************" << std::endl;
    std::cout << "Pantalla: " << pantalles.at(idScreen).getName() << std::endl;
    //printem els elements de la pantalla
    for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
        if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1 && ( pantalles.at(idScreen).pantallaelements[i].getName()=="Exit" || pantalles.at(idScreen).pantallaelements[i].getName()=="Info" || pantalles.at(idScreen).pantallaelements[i].getName()=="Shutdown" ) ) {
            std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
        } else if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1) {
            std::cout << "\t" << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
        } else {
            std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << std::endl;
        }
    }

    int control_bucle = 0;
    do {
        std::cin >> optionID;
        for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
            if ((pantalles.at(idScreen).pantallaelements[i].getClickable()==1) && (optionID == pantalles.at(idScreen).pantallaelements[i].getID())) {
                control_bucle = 1;
                break;
            }
         }
    }
    while( control_bucle == 0); //hem d'entrar un valor correcte
    optionName = pantalles.at(idScreen).pantallaelements[optionID].getName();
    std::cout << "JOC: "  << optionName << std::endl << std::endl;
    partida.setName(optionName);
    partida.removePlayers();

    if (optionName == "Shutdown") {
        std::cout << "destruir-ho tot i shutdown" << std::endl;
        systemShutdown = true;
    } else if (optionName == "Info") { //anar a INFO també significa acabar la partida
        std::cout << "INFO" << std::endl;
        appInfo = true;
    } else if (optionName == "Exit") {
        std::cout << "EXIT" << std::endl;
        gameExit = true;
    } else {
        idScreen = App::getScreenIDfromName("num_players");
        std::cout << "Pantalla: " << pantalles.at(idScreen).getName() << std::endl;
        //printem els elements de la pantalla
        for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
            if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1 && ( pantalles.at(idScreen).pantallaelements[i].getName()=="Exit" || pantalles.at(idScreen).pantallaelements[i].getName()=="Shutdown" ) ) {
                std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
            } else if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1) {
                std::cout << "\t" << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
            } else {
                std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << std::endl;
            }
        }
    }
}

void App::selectPlayers() {
    //seleccionem número de jugadors, i assignació dels avatars als jugadors
    idScreen = App::getScreenIDfromName("num_players");
    int control_bucle = 0;
    do {
        std::cin >> optionID;
        for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
            if ((pantalles.at(idScreen).pantallaelements[i].getClickable()==1) && (optionID == pantalles.at(idScreen).pantallaelements[i].getID())) {
                control_bucle = 1;
                break;
            }
        }
    }
    while( control_bucle == 0); //hem d'entrar un valor correcte
        
    std::cout << "Num Players: "  << optionID << std::endl << std::endl;
    partida.setNumPlayers(optionID);

    if (optionID == 6) {
        std::cout << "destruir-ho tot i shutdown" << std::endl;
        partida.removePlayers();
        systemShutdown = true;
    } else if (optionID == 5) {
        std::cout << "EXIT" << std::endl;
        partida.removePlayers();
        gameExit = true;
    } else {
        idScreen = App::getScreenIDfromName("assign_avatars");
        std::cout << "Pantalla: " << pantalles.at(idScreen).getName() << std::endl;

        //assignem els jugadors als avatars
        unsigned int numPlayersAssignats = 0;

        do {
            unsigned int repetit = 0;
            for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) { //recorrem els elements dels avatars, en la pantalla d'avatars
                if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1 && ( pantalles.at(idScreen).pantallaelements[i].getName()=="Exit" || pantalles.at(idScreen).pantallaelements[i].getName()=="Shutdown" ) ) {
                    std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
                } else if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1) {
                    std::cout << "\t" << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" ;
                } else if (pantalles.at(idScreen).pantallaelements[i].getClickable()==0 && pantalles.at(idScreen).pantallaelements[i].getName().find(".png") < 50) {
                    std::cout << " (" << pantalles.at(idScreen).pantallaelements[i].getName() << ")" << std::endl;
                } else {
                    std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << std::endl;
                }
            }

            std::cout << "Escull Jugador #" << numPlayersAssignats+1 << ":" << std::endl;
            control_bucle = 0;
            do {
                std::cin >> gameAvatarID;
                for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
                    if ((pantalles.at(idScreen).pantallaelements[i].getClickable()==1) && (gameAvatarID == pantalles.at(idScreen).pantallaelements[i].getID())) {
                        control_bucle = 1;
                        break;
                    }
                }
            }
            while( control_bucle == 0); //hem d'entrar un valor correcte

            gameAvatar = pantalles.at(idScreen).pantallaelements[gameAvatarID].getName();

            if (gameAvatar == "Shutdown") {
                std::cout << "destruir-ho tot i shutdown" << std::endl;
                partida.removePlayers();
                systemShutdown = true;
                break;
            } else if (gameAvatar == "Exit") {
                std::cout << "EXIT" << std::endl;
                partida.removePlayers();
                gameExit = true;
                break;
            } else if (gameAvatar != "Assign Avatar" ) {

                //comprovar que les assignacions no estiguin repetides
                for (unsigned int i=0; i<partida.jugador.size();i++) {
                    if (partida.jugador.at(i).getID() == gameAvatarID) repetit = 1;
                }
                if (repetit==0 ) {
                    partida.addPlayer(gameAvatarID, gameAvatar);
                    std::cout << "Jugador #" << numPlayersAssignats+1 << " - Avatar: "  << gameAvatar << std::endl << std::endl;
                    numPlayersAssignats++;
                }
            }
        } 
        while( numPlayersAssignats < optionID );

    }

}

void App::showInfo() {
    std::cout << "SHOW INFO: regles jocs, stats jocs, stats avatars" << std::endl;

    //mostrem INFO
    std::cout << "***************************************" << std::endl;
    idScreen = App::getScreenIDfromName("show_app_info");
    std::cout << "Pantalla: " << pantalles.at(idScreen).getName() << std::endl;
    //printem els elements de la pantalla
    for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
        if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1 && ( pantalles.at(idScreen).pantallaelements[i].getName()=="Exit" || pantalles.at(idScreen).pantallaelements[i].getName()=="Shutdown" ) ) {
            std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
        } else if (pantalles.at(idScreen).pantallaelements[i].getClickable()==1) {
            std::cout << "\t" << pantalles.at(idScreen).pantallaelements[i].getName() << " ("<< pantalles.at(idScreen).pantallaelements[i].getID() << ")" << std::endl;
        } else {
            std::cout << pantalles.at(idScreen).pantallaelements[i].getName() << std::endl;
        }
    }

    int control_bucle = 0;
    do {
        std::cin >> optionID;
        for (unsigned int i=0; i<pantalles.at(idScreen).pantallaelements.size(); i++) {
            if ((pantalles.at(idScreen).pantallaelements[i].getClickable()==1) && (optionID == pantalles.at(idScreen).pantallaelements[i].getID())) {
                control_bucle = 1;
                break;
            }
         }
    }
    while( control_bucle == 0); //hem d'entrar un valor correcte

    optionName = pantalles.at(idScreen).pantallaelements[optionID].getName();
    std::cout << "APP INFO OPTION: "  << optionName << std::endl << std::endl;

    if (optionName == "Shutdown") {
        std::cout << "destruir-ho tot i shutdown" << std::endl;
        partida.removePlayers();
        systemShutdown = true;
    } else if (optionName == "Exit") {
        std::cout << "EXIT" << std::endl;
        partida.removePlayers();
        gameExit = true;
    } else {
        //std::cout << "Pantalla: " << pantalles.at(idScreen).getName() << std::endl;
        //anem a mostrar la informació de l'opció escollida
        //std::cout << "APP INFO OPTION: "  << optionName << std::endl << std::endl;
        //r_301 -> he de veure que és 'r' (rules) i que és '301'

        std::vector<std::string> vec_opcio;
        vec_opcio = split(optionName, '_');

        if (vec_opcio.at(0)=="r") { //rules
            partida.setName(vec_opcio.at(1));
            partida.showGameRules();
        } else if (vec_opcio.at(0)=="sg") { //stat games
            partida.setName(vec_opcio.at(1));
            partida.showGameStats();
        } else if (vec_opcio.at(0)=="sp") { //stat player
            partida.addPlayer(0,vec_opcio.at(1));
            partida.jugador.at(0).showPlayerStats();
        }


    }

}

unsigned int App::getScreenIDfromName(std::string ScreenName) const {
    unsigned int id;
    id = 0;
    for(Screen scr : pantalles) {
        if (scr.getName() == ScreenName) {
            id = scr.getID();
            break;
        }
    }
    return id;
}

Screen App::getScreenfromName(std::string ScreenName) const {
    Screen screen;
    for(Screen scr : pantalles) {
        if (scr.getName() == ScreenName) {
            screen = scr;
            break;
        }
    }
    return screen; 
}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================

void App::play() {

    std::cout << "*** PLAY " << partida.getName() << " *************" << std::endl;
    gameNumPlayers = partida.jugador.size();
    //hem d'avariguar la pantalla on estan tots els elements del joc. La pantalla té el prefix 'game_'
    //std::cout << partida.getName() << std::endl;

    //construim la pantalla del joc al qual vull jugar
    Screen pantalla;

    if (pantalles.size()==4)  pantalles.erase(pantalles.begin() + 3); //Si ja havíem jugat, eliminem la pantalla del joc anterior abans de crear-ne una altra.
    pantalla.setID(4); //el 4 sempre serà el id de la pantalla
    pantalla.setName("game_" + partida.getName());
    pantalla.createScreenElements(pantalla.getName());
    pantalles.push_back(pantalla);

    //unsigned int getScreenIDfromName(std::string ScreenName) const;
    pantalla = getScreenfromName("game_" + partida.getName());
    //std::cout << pantalla.getName() << std::endl;

    //condicions originals de 301/501
    if (partida.getName() == "301" || partida.getName() == "301D") {
        for (unsigned int i=0;i<gameNumPlayers;i++) { //num_jugadors (de 1 a 4)
            puntsJugadorTotal[i] = 301;
            puntsJugadorTotalAntic[i] = 301;
        }
    } else if (partida.getName() == "501" || partida.getName() == "501D") {
        for (unsigned int i=0;i<gameNumPlayers;i++) { //num_jugadors (de 1 a 4)
            puntsJugadorTotal[i] = 501;
            puntsJugadorTotalAntic[i] = 501;
        }
    }

    App::play_Game();
}
// =============================================================================================

void App::play_Game() {
    int validScore;
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    App::updateScreenplay();

    while (gameExit == false) {

        std::string stroptionID = "";
        std::string stroptionIDType = ""; //és el prefix. s: score; u: interfície usuari

        int a = 0;
        int b = 0;

double wait_time = 3;            
clock_t start;
double diff;
start = clock(); 

        while(true) { //escollir una opció
            if (kbhit()) {
                a = getchar();
                if (a==10 && b==1) { //10: ENTER
                    break;
                } else {
                    stroptionID += a;
                    b=1;
                }
            }

diff = ( std::clock() - start ) / (double)CLOCKS_PER_SEC;
if (diff > wait_time) {
    //std::cout << "captura de frame" << std::endl;
    start = clock(); 
}
        }

        replace (stroptionID, "\n", "");

        stroptionIDType = stroptionID.substr (0,1); 
    
        if (stroptionIDType == "s") { //score
            isModifyingScore = false;
            stroptionID = stroptionID.substr (1,stroptionID.length()-1);
            strPuntsDiana = stroptionID;
            validScore = updateVarsScore();
            if (validScore == false) std::cout << "Score no correcte" << std::endl;
            if (validScore == true && numDart <= 3 && gameOver == false) App::logica(); //si estem en el 4rt dart, obligatòriament hem de pitjar/seleccionar Change player/round. Tampoc puc introduir puntuació si ja hem acabat.
        
        } else { //user interface
            optionID = atoi(stroptionID.c_str());
            optionName = pantalla.pantallaelements[optionID].getName();

            if (optionName == "Shutdown") {
                std::cout << "destruir-ho tot i shutdown" << std::endl;
                cleanVariables();
                partida.removePlayers();
                systemShutdown = true;
                gameExit = true;
            } else if (optionName == "Exit") {
                std::cout << "EXIT" << std::endl;
                cleanVariables();
                partida.removePlayers();
                gameExit = true;
            } else if (optionName == "Change player/round") {
                std::cout << "Change player/round" << std::endl;
                isModifyingScore = false;
                App::changePlayerRound();
            } else if (optionName == "Modify score") {
                std::cout << "MODIFY SCORE (" << strPuntsDiana << ")" << std::endl;
                //std::cout << puntsDiana << " " << isSector1 << " " << isSector2 << std::endl;
                strNearest = searchNearest();
                //std::cout << strNearest << std::endl;

                isModifyingScore = true;

                //gestió dels previs
                if (tornJugadorPrev == tornJugador && numRondaPrev == numRonda) {

                    //comuns
                    numDart = numDartPrev;
                    puntsJugadorDart[tornJugador-1][numDart-1] = -1;
                    strPuntsDiana = "";
                    puntsDiana = 0;
                    gameOver = gameOverPrev;
                    guanyador = guanyadorPrev;

                    //301, Cricket
                    puntsJugadorTotal[tornJugador-1] = puntsJugadorTotalPrev[tornJugador-1];
                    //Round the Clock
                    puntsJugadorRoundTheClock[tornJugador-1] = puntsJugadorRoundTheClockPrev[tornJugador-1];

                    //Cricket
                    JugadorCricketClosed[tornJugador-1] = JugadorCricketClosedPrev[tornJugador-1];
                    for (unsigned int j=1;j<=sizeof(cricketTargets)/sizeof(cricketTargets[0]);j++) { //recorrem els targets del Cricket
                        puntsJugadorCricket[tornJugador-1][j-1] = puntsJugadorCricketPrev[tornJugador-1][j-1];
                        puntsJugadorCricketSigne[tornJugador-1][j-1] = puntsJugadorCricketSignePrev[tornJugador-1][j-1];
                    }
                    //Count Up
                    for (unsigned int j=1;j<=sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);j++) { //recorrem els targets del Count Up
                        puntsJugadorRonda[tornJugador-1][j-1] = puntsJugadorRondaPrev[tornJugador-1][j-1];
                    }

                    //Halve It
                    for (unsigned int j=1;j<=sizeof(puntsJugadorHalveIt[0])/sizeof(puntsJugadorHalveIt[0][0]);j++) { //recorrem els targets del Halve It
                        puntsJugadorHalveIt[tornJugador-1][j-1] = puntsJugadorHalveItPrev[tornJugador-1][j-1];
                    }

                    App::updateScreenplay();
                }

            } 

        }
    }
}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================


void App::updateScreenplay() { //genèric
    if (partida.getName() == "301" || partida.getName() == "301D" || partida.getName() == "501" || partida.getName() == "501D") {
        App::updateScreenplay_301501();
    } else if (partida.getName() == "Count Up") {
        App::updateScreenplay_Count_Up();
    } else if (partida.getName() == "Cricket") {
        App::updateScreenplay_Cricket();
    }  else if (partida.getName() == "Round the Clock") {
        App::updateScreenplay_Round_the_Clock();
    }   else if (partida.getName() == "Halve It") {
        App::updateScreenplay_Halve_It();
    } 
}

// =============================================================================================

void App::updateScreenplay_301501() {
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
    std::cout << partida.getName() << ". " << "RONDA #" << numRonda << std::endl;
    std::cout << "=============================" << std::endl; 


    //numRonda
    scr_ele = pantalla.getScreenElementfromName("txt_numRonda");
    std::cout << scr_ele.getName() << ": "<< yellow_chars << numRonda << normal_chars << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< yellow_chars << tornJugador << normal_chars << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsDiana << normal_chars << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsGuanyador << normal_chars << std::endl;
    }

    for (unsigned int i=1;i<=partida.jugador.size();i++) {
        //jugadorx
        scr_ele = pantalla.getScreenElementfromName("txt_picjugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatarPic() << std::endl;

        scr_ele = pantalla.getScreenElementfromName("txt_jugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatar() << std::endl;

        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //recorrem els darts
            //puntsDart1Jugadorx
            scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            if (puntsJugadorDart[i-1][j] == -1) {
                std::cout << scr_ele.getName() << ": " << std::endl;
            } else {
                std::cout << scr_ele.getName() << ": " << red_chars << puntsJugadorDart[i-1][j] << normal_chars << std::endl;
            }
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << blue_chars << puntsJugadorTotal[i-1] << normal_chars << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if (isBusts == true) {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "BUSTS" << normal_chars << std::endl;
    } else if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
    }  else if (gameOver == false) {
        if (isModifyingScore) {
            std::cout << scr_ele.getName() << ": " << "Rectify: " << cyan_chars << strNearest << normal_chars << std::endl;        
        } else {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "END GAME. WINNER:" << normal_chars << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << partida.jugador.at(guanyador).getAvatar() << normal_chars << std::endl;
    }

    //mostrem informació de com s'ha d'introduir els punts
    std::cout << std::endl << "Prefix 's' for score: (sXX, sXX_1, sXX_2, sDXX, sTXX, sSB, sDB, s0, sMISS)" << std::endl;

    //Change player/round
    scr_ele = pantalla.getScreenElementfromName("Change player/round");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Modify score
    scr_ele = pantalla.getScreenElementfromName("Modify score");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Exit
    scr_ele = pantalla.getScreenElementfromName("Exit");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Shutdown
    scr_ele = pantalla.getScreenElementfromName("Shutdown");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Retroplaneta.com
    scr_ele = pantalla.getScreenElementfromName("Retroplaneta.com");
    std::cout << scr_ele.getName() << std::endl;
}

// =============================================================================================

void App::updateScreenplay_Count_Up() {
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
    std::cout << partida.getName() << ". " << "RONDA #" << numRonda << std::endl;
    std::cout << "=============================" << std::endl; 


    //numRonda
    scr_ele = pantalla.getScreenElementfromName("txt_numRonda");
    std::cout << scr_ele.getName() << ": "<< numRonda << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< tornJugador << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsDiana << normal_chars << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsGuanyador << normal_chars << std::endl;
    }

    for (unsigned int i=1;i<=partida.jugador.size();i++) {
        //jugadorx
        scr_ele = pantalla.getScreenElementfromName("txt_picjugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatarPic() << std::endl;

        scr_ele = pantalla.getScreenElementfromName("txt_jugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatar() << std::endl;

        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //recorrem els darts
            //puntsDart1Jugadorx
            scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            if (puntsJugadorDart[i-1][j] == -1) {
                std::cout << scr_ele.getName() << ": " << std::endl;
            } else {
                std::cout << scr_ele.getName() << ": " << red_chars << puntsJugadorDart[i-1][j] << normal_chars << std::endl;
            }
        }

        for (unsigned int j=0;j<sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);j++) { //recorrem les 8 rondes
            //puntsJugadorRonda txt_puntsRonda4Jugador" + int_to_string(i)
            //scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            scr_ele = pantalla.getScreenElementfromName("txt_puntsRonda" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << green_chars << puntsJugadorRonda[i-1][j] << normal_chars << std::endl;
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << blue_chars << puntsJugadorTotal[i-1] << normal_chars << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
    } else if (gameOver == false) {
        if (isModifyingScore) {
            std::cout << scr_ele.getName() << ": " << "Rectify: " << strNearest << std::endl;        
        } else {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "END GAME. WINNER:" << normal_chars << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << partida.jugador.at(guanyador).getAvatar() << normal_chars << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << "Prefix 's' for score: (sXX, sDXX, sTXX, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/round
    scr_ele = pantalla.getScreenElementfromName("Change player/round");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Modify score
    scr_ele = pantalla.getScreenElementfromName("Modify score");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;
    
    //Exit
    scr_ele = pantalla.getScreenElementfromName("Exit");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Shutdown
    scr_ele = pantalla.getScreenElementfromName("Shutdown");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Retroplaneta.com
    scr_ele = pantalla.getScreenElementfromName("Retroplaneta.com");
    std::cout << scr_ele.getName() << std::endl;
}

// =============================================================================================

void App::updateScreenplay_Cricket() {
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
    std::cout << partida.getName() << ". " << "RONDA #" << numRonda << std::endl;
    std::cout << "=============================" << std::endl; 


    //numRonda
    scr_ele = pantalla.getScreenElementfromName("txt_numRonda");
    std::cout << scr_ele.getName() << ": "<< numRonda << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< tornJugador << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsDiana << normal_chars << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsGuanyador << normal_chars << std::endl;
    }

    for (unsigned int i=1;i<=partida.jugador.size();i++) {
        //jugadorx
        scr_ele = pantalla.getScreenElementfromName("txt_picjugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatarPic() << std::endl;

        scr_ele = pantalla.getScreenElementfromName("txt_jugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatar() << std::endl;

        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //recorrem els dards
            //puntsDart1Jugadorx
            scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            if (puntsJugadorDart[i-1][j] == -1) {
                std::cout << scr_ele.getName() << ": " << std::endl;
            } else {
                std::cout << scr_ele.getName() << ": " << red_chars << puntsJugadorDart[i-1][j] << normal_chars << std::endl;
            }
        }

        for (unsigned int j=1;j<=sizeof(cricketTargets)/sizeof(cricketTargets[0]);j++) { //recorrem els targets del Cricket
            //puntsJugadorRonda
            scr_ele = pantalla.getScreenElementfromName("txt_puntsCricket" + int_to_string(j) + "Jugador" + int_to_string(i));
            std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << green_chars << puntsJugadorCricket[i-1][j-1] << " " << puntsJugadorCricketSigne[i-1][j-1] << normal_chars << std::endl;
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << blue_chars << puntsJugadorTotal[i-1] << normal_chars << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ( (strPuntsDiana == "MISS" || strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
    } else if (gameOver == false) {
        if (isModifyingScore) {
            std::cout << scr_ele.getName() << ": " << "Rectify: " << strNearest << std::endl;        
        } else {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "END GAME. WINNER:" << normal_chars << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << partida.jugador.at(guanyador).getAvatar() << normal_chars << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << "Prefix 's' for score: (sXX, sDXX, sTXX, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/round
    scr_ele = pantalla.getScreenElementfromName("Change player/round");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Modify score
    scr_ele = pantalla.getScreenElementfromName("Modify score");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;
    
    //Exit
    scr_ele = pantalla.getScreenElementfromName("Exit");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Shutdown
    scr_ele = pantalla.getScreenElementfromName("Shutdown");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Retroplaneta.com
    scr_ele = pantalla.getScreenElementfromName("Retroplaneta.com");
    std::cout << scr_ele.getName() << std::endl;
}

// =============================================================================================

void App::updateScreenplay_Round_the_Clock() {
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
    std::cout << partida.getName() << ". " << "RONDA #" << numRonda << std::endl;
    std::cout << "=============================" << std::endl; 


    //numRonda
    scr_ele = pantalla.getScreenElementfromName("txt_numRonda");
    std::cout << scr_ele.getName() << ": "<< numRonda << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< tornJugador << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsDiana << normal_chars << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsGuanyador << normal_chars << std::endl;
    }

    for (unsigned int i=1;i<=partida.jugador.size();i++) {
        //jugadorx
        scr_ele = pantalla.getScreenElementfromName("txt_picjugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatarPic() << std::endl;

        scr_ele = pantalla.getScreenElementfromName("txt_jugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatar() << std::endl;

        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //recorrem els darts
            //puntsDart1Jugadorx
            scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            if (puntsJugadorDart[i-1][j] == -1) {
                std::cout << scr_ele.getName() << ": " << std::endl;
            } else {
                std::cout << scr_ele.getName() << ": " << red_chars << puntsJugadorDart[i-1][j] << normal_chars << std::endl;
            }
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << blue_chars << roundtheclockTargets[puntsJugadorRoundTheClock[i-1]] << normal_chars << std::endl; 
    }

    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
    }  else if (gameOver == false) {
        if (isModifyingScore) {
            std::cout << scr_ele.getName() << ": " << "Rectify: " << strNearest << std::endl;        
        } else {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "END GAME. WINNER:" << normal_chars << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << partida.jugador.at(guanyador).getAvatar() << normal_chars << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << "Prefix 's' for score: (sXX, sDXX, sTXX, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/round
    scr_ele = pantalla.getScreenElementfromName("Change player/round");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Modify score
    scr_ele = pantalla.getScreenElementfromName("Modify score");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Exit
    scr_ele = pantalla.getScreenElementfromName("Exit");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Shutdown
    scr_ele = pantalla.getScreenElementfromName("Shutdown");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Retroplaneta.com
    scr_ele = pantalla.getScreenElementfromName("Retroplaneta.com");
    std::cout << scr_ele.getName() << std::endl;
}

// =============================================================================================

void App::updateScreenplay_Halve_It() {
    Screen pantalla;
    ScreenElement scr_ele;
    pantalla = getScreenfromName("game_" + partida.getName());

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
    std::cout << partida.getName() << ". " << "RONDA #" << numRonda << std::endl;
    std::cout << "=============================" << std::endl; 

    //numRonda
    scr_ele = pantalla.getScreenElementfromName("txt_numRonda");
    std::cout << scr_ele.getName() << ": "<< numRonda << ": " << halveitTargets[numRonda-1] << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< tornJugador << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsDiana << normal_chars << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << puntsGuanyador << normal_chars << std::endl;
    }

    for (unsigned int i=1;i<=partida.jugador.size();i++) {
        //jugadorx
        scr_ele = pantalla.getScreenElementfromName("txt_picjugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatarPic() << std::endl;

        scr_ele = pantalla.getScreenElementfromName("txt_jugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << i << ". " << partida.jugador.at(i-1).getAvatar() << std::endl;

        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //recorrem els dards
            //puntsDart1Jugadorx
            scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            if (puntsJugadorDart[i-1][j] == -1) {
                std::cout << scr_ele.getName() << ": " << std::endl;
            } else {
                std::cout << scr_ele.getName() << ": " << red_chars << puntsJugadorDart[i-1][j] << normal_chars << std::endl;
            }
        }

        for (unsigned int j=1;j<=sizeof(halveitTargets)/sizeof(halveitTargets[0]);j++) { //recorrem els targets del Cricket
            //puntsJugadorRonda
            scr_ele = pantalla.getScreenElementfromName("txt_puntsHalveIt" + int_to_string(j) + "Jugador" + int_to_string(i));
            if (puntsJugadorHalveIt[i-1][j-1] != -1) {
                std::cout << scr_ele.getName() << " (" << scr_ele.getName2() << "): " << green_chars << puntsJugadorHalveIt[i-1][j-1] << normal_chars << std::endl;
            } else {
                std::cout << scr_ele.getName() << " (" << scr_ele.getName2() << "): " << std::endl;                
            }
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << blue_chars << puntsJugadorTotal[i-1] << normal_chars << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
    } else if (gameOver == false) {
        if (isModifyingScore) {
            std::cout << scr_ele.getName() << ": " << "Rectify: " << strNearest << std::endl;        
        } else {
            std::cout << scr_ele.getName() << ": " << yellow_backg_chars << strPuntsDiana << normal_chars << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< yellow_backg_chars << "END GAME. WINNER:" << normal_chars << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << yellow_backg_chars << partida.jugador.at(guanyador).getAvatar() << normal_chars << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << "Prefix 's' for score: (sXX, sDXX, sTXX, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/round
    scr_ele = pantalla.getScreenElementfromName("Change player/round");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Modify score
    scr_ele = pantalla.getScreenElementfromName("Modify score");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;
    
    //Exit
    scr_ele = pantalla.getScreenElementfromName("Exit");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Shutdown
    scr_ele = pantalla.getScreenElementfromName("Shutdown");
    std::cout << scr_ele.getName() << " ("<< scr_ele.getID() << ")" << std::endl;

    //Retroplaneta.com
    scr_ele = pantalla.getScreenElementfromName("Retroplaneta.com");
    std::cout << scr_ele.getName() << std::endl;
}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================


void cleanVariables() {
    tornJugador = 1;
    numRonda = 1;
    numDart = 1;
    puntsDiana = 0;
    strPuntsDiana = "";
    isDianaDouble = false;
    isDianaTreble = false;
    isSingleBull = false;
    isDoubleBull = false;
    isSector1 = false;
    isSector2 = false;
    isBusts = false;
    isMiss = true;
    isModifyingScore = false;
    
    guanyador = -1;
    puntsGuanyador = 0;
    gameOver = false;

    numDartPrev = 1;
    tornJugadorPrev = 1;
    numRondaPrev = 1;
    gameOverPrev = false;
    puntsGuanyadorPrev = 0;
    guanyadorPrev = -1;


    for (unsigned int i=0;i<gameNumPlayers;i++) { //num_jugadors (de 1 a 4)
        for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //3
            puntsJugadorDart[i][j] = -1;
        }
        puntsJugadorTotal[i] = 0;
        puntsJugadorTotalPrev[i] = 0;
    }

    //Count Up
    for (unsigned int i=0;i<sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);i++) { //num_rondes
        for (unsigned int j=0;j<gameNumPlayers;j++) { //num_jugadors
            puntsJugadorRonda[j][i] = 0;
            puntsJugadorRondaPrev[j][i] = 0;
        }
    }

    //Cricket
    for (unsigned int i=0;i<sizeof(puntsJugadorCricket[0])/sizeof(puntsJugadorCricket[0][0]);i++) { //num_rondes
        for (unsigned int j=0;j<gameNumPlayers;j++) { //num_jugadors
            puntsJugadorCricket[j][i] = 0;
            puntsJugadorCricketSigne[j][i] = "";
            puntsJugadorCricketPrev[j][i] = 0;
            puntsJugadorCricketSignePrev[j][i] = "";
            if (i==0) JugadorCricketClosed[j] = false;
        }
    }
    isMissCricket = true;

    //Round The Clock
    for (unsigned int j=0;j<gameNumPlayers;j++) { //num_jugadors
        puntsJugadorRoundTheClock[j] = 0;
        puntsJugadorRoundTheClockPrev[j] = 0;
    }

    //Halve It
    for (unsigned int i=0;i<sizeof(puntsJugadorCricket[0])/sizeof(puntsJugadorCricket[0][0]);i++) { //num_rondes
        for (unsigned int j=0;j<gameNumPlayers;j++) { //num_jugadors
            puntsJugadorHalveIt[j][i] = -1;
        }
    }
}

// =============================================================================================

int updateVarsScore() {

    int validScore = true;

    std::string strTemp;
    strTemp = strPuntsDiana;

    if (strPuntsDiana == "SB") {
        puntsDiana = 25;
        isSingleBull = true;
        isDoubleBull = false;
        isDianaDouble = false;
        isDianaTreble = false;
        isSector1 = false;
        isSector2 = false;
        isMiss = false;
    }   else if (strPuntsDiana == "DB") {
        puntsDiana = 50;
        isSingleBull = false;
        isDoubleBull = true;
        isDianaDouble = false;
        isDianaTreble = false;
        isSector1 = false;
        isSector2 = false;
        isMiss = false;
    } else if (strPuntsDiana.find("D") != std::string::npos) {
        replace (strTemp, "D", "");
        puntsDiana = atoi(strTemp.c_str())*2;
        isSingleBull = false;
        isDoubleBull = false;
        isDianaDouble = true;
        isDianaTreble = false;
        isSector1 = false;
        isSector2 = false;
        isMiss = false;
        if (puntsDiana > 40 || puntsDiana <0 ) validScore = false; 
    } else if (strPuntsDiana.find("T") != std::string::npos) {
        replace (strTemp, "T", "");
        puntsDiana = atoi(strTemp.c_str())*3;
        isSingleBull = false;
        isDoubleBull = false;
        isDianaDouble = false;
        isDianaTreble = true;
        isSector1 = false;
        isSector2 = false;
        isMiss = false;
        if (puntsDiana > 60 || puntsDiana <0 ) validScore = false; 
    } else {
        isDianaDouble = false;
        isDianaTreble = false;
        isMiss = false;
        isSingleBull = false;
        isDoubleBull = false; 
        isSector1 = false; 
        isSector2 = false;
        isSector1 = false;
        isSector2 = false;

        if (strPuntsDiana.find("_1") != std::string::npos) {
            replace (strPuntsDiana, "_1", "");
            isSector1 = true;
            puntsDiana = atoi(strPuntsDiana.c_str());
        } else if (strPuntsDiana.find("_2") != std::string::npos) {
            replace (strPuntsDiana, "_2", "");
            isSector2 = true;
            puntsDiana = atoi(strPuntsDiana.c_str());
        } else {
            puntsDiana = atoi(strPuntsDiana.c_str());
        }

        if (puntsDiana == 0) {
            isMiss = true;
            strPuntsDiana = "MISS";
        } else if (puntsDiana == 25) { 
            isSingleBull = true;
            strPuntsDiana = "SB";
        } else if (puntsDiana == 50) { 
            isDoubleBull = true;
            strPuntsDiana = "DB";
        } else if (puntsDiana > 20 || puntsDiana <0 ) {
            validScore = false; 
        }

    }

    return validScore;
}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================


void App::logica() {

    if (partida.getName() == "301" || partida.getName() == "301D" || partida.getName() == "501" || partida.getName() == "501D") {
        App::logica_301501();
    } else if (partida.getName() == "Count Up") {
        App::logica_Count_Up();
    } else if (partida.getName() == "Cricket") {
        App::logica_Cricket();
    }  else if (partida.getName() == "Round the Clock") {
        App::logica_Round_the_Clock();
    }   else if (partida.getName() == "Halve It") {
        App::logica_Halve_It();
    } 

}

// =============================================================================================

void App::logica_301501() {

    puntsJugadorDart[tornJugador-1][numDart-1] = puntsDiana;

    if (puntsJugadorTotal[tornJugador-1] <  puntsDiana) {
        isBusts = true;
        puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
        puntsJugadorTotal[tornJugador-1] = puntsJugadorTotalAntic[tornJugador-1];
    } else if ((strPuntsDiana.find("D") != std::string::npos) && (partida.getName().find("D") != std::string::npos) && (strPuntsDiana != "DB")) { //DB conté D!!
        puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
        puntsJugadorTotal[tornJugador-1] -= puntsDiana;
        if (numDart == 3) puntsJugadorTotalAntic[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
        if (puntsJugadorTotal[tornJugador-1] == 0) {
            gameOverPrev = gameOver;
            gameOver = true;
        }
    } else { //cas normal, no és doble
        if (partida.getName().find("D") != std::string::npos) { //si és 301D o 501D, no podem acabar la partida amb single
            if (puntsJugadorTotal[tornJugador-1] == puntsDiana) {
                isBusts = true;
                puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
                puntsJugadorTotal[tornJugador-1] = puntsJugadorTotalAntic[tornJugador-1];
            } else if ((puntsJugadorTotal[tornJugador-1] - puntsDiana)==1) { //no em puc quedar amb 1 punt, no es podria tancar amb un doble.
                isBusts = true;
                puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
                puntsJugadorTotal[tornJugador-1] = puntsJugadorTotalAntic[tornJugador-1];
            } else {
                puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
                puntsJugadorTotal[tornJugador-1] -= puntsDiana;
                if (numDart == 3) puntsJugadorTotalAntic[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
            }
        } else { //si és 301 o 501, podem acabar la partida amb single
            puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
            puntsJugadorTotal[tornJugador-1] -= puntsDiana;
            if (numDart == 3) puntsJugadorTotalAntic[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
            if (puntsJugadorTotal[tornJugador-1] == 0) {
                gameOverPrev = gameOver;
                gameOver = true;
            }
        }

    }

    numDartPrev = numDart;
    if (gameOver == true) {
        std::cout << "Final de la partida" << std::endl;
        guanyadorPrev = guanyador;
        guanyador = tornJugador-1;

    } else {

        //numDartPrev = numDart;
        if (isBusts == true) {
            numDart = 3;
        }
        numDart++;
        tornJugadorPrev = tornJugador;
        numRondaPrev = numRonda;
    }

    App::updateScreenplay();
    isBusts = false;
}

// =============================================================================================

void App::logica_Count_Up() {
    puntsJugadorDart[tornJugador-1][numDart-1] = puntsDiana;

    for (unsigned int j=1;j<=sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);j++) { //recorrem els targets del Cricket
        puntsJugadorRondaPrev[tornJugador-1][j-1] = puntsJugadorRonda[tornJugador-1][j-1];
    }

    puntsJugadorRonda[tornJugador-1][numRonda-1] += puntsDiana;
    puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
    puntsJugadorTotal[tornJugador-1] += puntsDiana;

    numDartPrev = numDart;
    numDart++;
    tornJugadorPrev = tornJugador;
    numRondaPrev = numRonda;

    App::updateScreenplay_Count_Up();
}

// =============================================================================================

void App::logica_Cricket() {

    isMissCricket = true;

    puntsJugadorDart[tornJugador-1][numDart-1] = puntsDiana;

    unsigned int valcricketTarget = 0;
    for (unsigned int i = 0; i < sizeof(cricketTargets)/sizeof(cricketTargets[0])-1;i++ ) { //en el cricket, l'últim valor és el cas especial, és el Bull (B)
        puntsJugadorCricketPrev[tornJugador-1][i] = puntsJugadorCricket[tornJugador-1][i];
        puntsJugadorCricketSignePrev[tornJugador-1][i] = puntsJugadorCricketSigne[tornJugador-1][i];
        valcricketTarget = atoi(cricketTargets[i].c_str());
        if ( (puntsDiana == valcricketTarget && isDianaDouble == false) || (puntsDiana == valcricketTarget*2 && isDianaDouble == true ) || (puntsDiana == valcricketTarget*3 && isDianaTreble == true)) {
            isMissCricket = false;
            if (puntsDiana == valcricketTarget) {
                puntsJugadorCricket[tornJugador-1][i]++;
            } else if ( puntsDiana == valcricketTarget*2) {
                puntsJugadorCricket[tornJugador-1][i]+=2;
                std::cout << puntsJugadorCricketPrev[tornJugador-1][i]  << "===" << puntsJugadorCricket[tornJugador-1][i] << std::endl;
            } else if ( puntsDiana == valcricketTarget*3) {
                puntsJugadorCricket[tornJugador-1][i]+=3;
                std::cout << puntsJugadorCricketPrev[tornJugador-1][i]  << "===" << puntsJugadorCricket[tornJugador-1][i] << std::endl;
            }

            if (puntsJugadorCricket[tornJugador-1][i] > 3 && puntsJugadorCricketPrev[tornJugador-1][i] <=3) {              
                puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
                puntsJugadorTotal[tornJugador-1] += (puntsJugadorCricket[tornJugador-1][i]-3) * valcricketTarget;
            } else if (puntsJugadorCricket[tornJugador-1][i] > 3 && puntsJugadorCricketPrev[tornJugador-1][i] > 3) {     
                puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
                puntsJugadorTotal[tornJugador-1] += (puntsJugadorCricket[tornJugador-1][i]-puntsJugadorCricketPrev[tornJugador-1][i]) * valcricketTarget;
            }
        }
        if (puntsJugadorCricket[tornJugador-1][i] == 0) {
            puntsJugadorCricketSigne[tornJugador-1][i] = "";
        } else if (puntsJugadorCricket[tornJugador-1][i] == 1) {
            puntsJugadorCricketSigne[tornJugador-1][i] = "I";
        } else if (puntsJugadorCricket[tornJugador-1][i] == 2) {
            puntsJugadorCricketSigne[tornJugador-1][i] = "II";
        } else {
            puntsJugadorCricketSigne[tornJugador-1][i] = "III";
        }
    }

    //cas target B (cricketTargets[6]) (cas especial: SB, DB)
    if ( puntsDiana == 25 || puntsDiana == 50) {
        isMissCricket = false;
        //sizeof(cricketTargets)/sizeof(cricketTargets[0])-1 : en el cas normal aquest valor és 6, que és l'index del cas espedcia Bull's Eye. Però fico el carro gros perquè en la forma escurçada l'index és 2
        unsigned int num_index_bulls_eye = sizeof(cricketTargets)/sizeof(cricketTargets[0])-1;
        puntsJugadorCricketPrev[tornJugador-1][num_index_bulls_eye] = puntsJugadorCricket[tornJugador-1][num_index_bulls_eye];
        puntsJugadorCricketSignePrev[tornJugador-1][num_index_bulls_eye] = puntsJugadorCricketSigne[tornJugador-1][num_index_bulls_eye];

        if (puntsDiana == 25) {
            puntsJugadorCricket[tornJugador-1][num_index_bulls_eye]++;
        } else if ( puntsDiana == 50) {
            puntsJugadorCricket[tornJugador-1][num_index_bulls_eye]+=2;
        }

        if (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye] > 3 && puntsJugadorCricketPrev[tornJugador-1][num_index_bulls_eye] <=3) {              
            puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
            puntsJugadorTotal[tornJugador-1] += (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye]-3) * 25;
        } else if (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye] > 3 && puntsJugadorCricketPrev[tornJugador-1][num_index_bulls_eye] > 3) {     
            puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];
            puntsJugadorTotal[tornJugador-1] += (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye]-puntsJugadorCricketPrev[tornJugador-1][num_index_bulls_eye]) * 25;
        }

        if (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye] == 0) {
            puntsJugadorCricketSigne[tornJugador-1][num_index_bulls_eye] = "";
        } else if (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye] == 1) {
            puntsJugadorCricketSigne[tornJugador-1][num_index_bulls_eye] = "I";
        } else if (puntsJugadorCricket[tornJugador-1][num_index_bulls_eye] == 2) {
            puntsJugadorCricketSigne[tornJugador-1][num_index_bulls_eye] = "II";
        } else {
            puntsJugadorCricketSigne[tornJugador-1][num_index_bulls_eye] = "III";
        }

    }         

    if (isMissCricket) strPuntsDiana = "MISS";

    numDartPrev = numDart;
    numDart++;
    tornJugadorPrev = tornJugador;
    numRondaPrev = numRonda;

    App::updateScreenplay();

}

// =============================================================================================

void App::logica_Round_the_Clock() {
    unsigned int puntsActual = 0;
    puntsJugadorDart[tornJugador-1][numDart-1] = puntsDiana;
                        
    //if (puntsJugadorRoundTheClock[tornJugador-1] < 20) { //del 1 al 20
    if (puntsJugadorRoundTheClock[tornJugador-1] < sizeof(roundtheclockTargets)/sizeof(roundtheclockTargets[0]) -2 ) { //del 1 al 20
        puntsActual = (unsigned int)atoi(roundtheclockTargets[puntsJugadorRoundTheClock[tornJugador-1]].c_str());
        if (puntsActual == puntsDiana || (puntsActual*2 == puntsDiana && isDianaDouble==true) || (puntsActual*3 == puntsDiana && isDianaTreble==true)) { //encertem target
            puntsJugadorRoundTheClockPrev[tornJugador-1] = puntsJugadorRoundTheClock[tornJugador-1];
            puntsJugadorRoundTheClock[tornJugador-1] ++;
        }
    } else { //per acabar, Bull's Eye (25 o 50)


        if (puntsDiana == 25 || puntsDiana == 50) {

            puntsJugadorRoundTheClockPrev[tornJugador-1] = puntsJugadorRoundTheClock[tornJugador-1];
            puntsJugadorRoundTheClock[tornJugador-1] ++;

            gameOverPrev = gameOver;
            gameOver = true;
            guanyadorPrev = guanyador;
            guanyador = tornJugador-1;

            numDartPrev = numDart;
            numDart++;

            tornJugadorPrev = tornJugador;
            numRondaPrev = numRonda;
        }
    }
    
    if (gameOver==false) {
        numDartPrev = numDart;
        numDart++;
        tornJugadorPrev = tornJugador;
        numRondaPrev = numRonda;
    }

    App::updateScreenplay();
}

// =============================================================================================

void App::logica_Halve_It() {
    unsigned int puntsTargetRonda = 0;

    puntsJugadorDart[tornJugador-1][numDart-1] = puntsDiana;
    puntsJugadorTotalPrev[tornJugador-1] = puntsJugadorTotal[tornJugador-1];

    for (unsigned int j=1;j<=sizeof(puntsJugadorHalveIt[0])/sizeof(puntsJugadorHalveIt[0][0]);j++) { //recorrem els targets del Halve It
        puntsJugadorHalveItPrev[tornJugador-1][j-1] = puntsJugadorHalveIt[tornJugador-1][j-1];
    }

    puntsTargetRonda = (unsigned int)atoi(halveitTargets[numRonda-1].c_str());
    if (puntsTargetRonda == puntsDiana || (puntsTargetRonda*2 == puntsDiana && isDianaDouble==true) || (puntsTargetRonda*3 == puntsDiana && isDianaTreble==true)) { //encertem target
        puntsJugadorTotal[tornJugador-1] += puntsDiana;
        if (puntsJugadorHalveIt[tornJugador-1][numRonda-1] == -1) puntsJugadorHalveIt[tornJugador-1][numRonda-1] = 1;
    } else { //no encertem target
        if (puntsJugadorHalveIt[tornJugador-1][numRonda-1] == -1 && numDart == 3) {
            puntsJugadorHalveIt[tornJugador-1][numRonda-1] = 0;
            puntsJugadorTotal[tornJugador-1] = puntsJugadorTotal[tornJugador-1]/2; //penalització Halve It
        }
    }

    numDartPrev = numDart;
    numDart++;
    tornJugadorPrev = tornJugador;
    numRondaPrev = numRonda;
    App::updateScreenplay();

}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================

void App::changePlayerRound() {
    //en el Halve It i Count Up, el joc s'acaba quan s'acaben les rondes (i els torns), i guanya qui té més punts.
    //en el Cricket per acabar la partida hem d'esperar que s'acabi el torn.
    //en el 301 i Round the clock la partida s'acaba amb el primer que aconseguei l'objectiu (no cal acabar la ronda)

    if (numDart > 3 && gameOver == false ) {
        numDart = 1;

        tornJugadorPrev = tornJugador;
        numRondaPrev = numRonda;

        tornJugador++;
        if (tornJugador <= gameNumPlayers) {
            std::cout << "WAIT..." << std::endl;
            sleep(1); //espera per veure els resultats, abans d'anar al següent jugador (quant tingui un botó ja no caldrà)
            isBusts = false;
        } else {

            tornJugadorPrev = tornJugador;
            numRondaPrev = numRonda;

            tornJugador = 1;
            numRonda++;
            std::cout << "WAIT..." << std::endl;
            sleep(1); //espera per veure els resultats, abans d'anar a la següent ronda (quant tingui un botó ja no caldrà)
            puntsDiana = 0;
            strPuntsDiana = "";

            for (unsigned int i=0;i<gameNumPlayers;i++) { //num_jugadors (de 1 a 4)
                for (unsigned int j=0;j<sizeof(puntsJugadorDart[0])/sizeof(puntsJugadorDart[0][0]);j++) { //3
                    puntsJugadorDart[i][j] = -1;
                }
            }
        }

        //FINAL DE PARTIDA
        if (partida.getName() == "301" || partida.getName() == "301D" || partida.getName() == "501" || partida.getName() == "501D") {
            //
        } else if (partida.getName() == "Count Up") {
            if (numRonda > sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0])) { //num_rondes_max
                gameOver = true;
                std::cout << "Final de la partida" << std::endl;
                for (unsigned int i=0;i<partida.jugador.size();i++) { //4
                    if (puntsJugadorTotal[i] > puntsGuanyador) {
                        guanyador = i;
                        puntsGuanyador = puntsJugadorTotal[i];
                    }
                }

                numRondaPrev = numRonda;
                tornJugadorPrev = tornJugador;

                numRonda--;
                tornJugador = gameNumPlayers;

            }
        } else if (partida.getName() == "Cricket") {
            //comprovació de final de la partida. Estem al final de la ronda.
            //hem de recórrer tots els jugadors. Guanya la persona que ha tancat tots els targets, i que té més punts
            puntsGuanyadorPrev = puntsGuanyador;
            puntsGuanyador = 0;
            unsigned int isJugadorClosed;
            for (unsigned int i=0;i<gameNumPlayers;i++) { //num_jugadors (de 1 a 4)
                isJugadorClosed = true;
                //si els punts del jugador > puntsGuanyador, actualitzo puntsGuanyador
                if (puntsJugadorTotal[i] >= puntsGuanyador) {
                    puntsGuanyadorPrev = puntsGuanyador;
                    puntsGuanyador = puntsJugadorTotal[i];
                    guanyadorPrev = guanyador;
                    guanyador = -1;
                }
                //comprovar que hem tancat tots els targets
                for (unsigned int j=0;j<sizeof(cricketTargets)/sizeof(cricketTargets[0]);j++) { 
                    if (puntsJugadorCricket[i][j] < 3) isJugadorClosed = false;
                }
                //si hem tancat tots els targets i puntsJugador = puntsGuanyador, ficar guanyador=i
                if (isJugadorClosed == true && puntsGuanyador == puntsJugadorTotal[i]) {
                    guanyadorPrev = guanyador;
                    guanyador = i;
                }
            }
            //si guanyador>=0, ja tenim guanyador i podem fer gameover.
            if (guanyador >=0 ) {
                gameOverPrev = gameOver;
                gameOver = true;
                std::cout << "Final de la partida" << std::endl;
                //restituïm les variables per a la presentació final dels resultats
                numRondaPrev = numRonda;
                tornJugadorPrev = tornJugador;
                numRonda--;
                tornJugador = gameNumPlayers;
            }
        }  else if (partida.getName() == "Round the Clock") {
            //
        }   else if (partida.getName() == "Halve It") {
            if (numRonda > sizeof(puntsJugadorHalveIt[0])/sizeof(puntsJugadorHalveIt[0][0])) { //num_rondes_max
                gameOver = true;
                std::cout << "Final de la partida" << std::endl;
                for (unsigned int i=0;i<partida.jugador.size();i++) { //4
                    if (puntsJugadorTotal[i] > puntsGuanyador) {
                        guanyador = i;
                        puntsGuanyador = puntsJugadorTotal[i];
                    }
                }
                //restituïm les variables per a la presentació final dels resultats
                numRondaPrev = numRonda;
                tornJugadorPrev = tornJugador;

                numRonda--;
                tornJugador = gameNumPlayers;

            }
        }

        //actualitzem pantalla
        App::updateScreenplay();
    }
}

// ==================================================================================================================================================================
// ==================================================================================================================================================================
// ==================================================================================================================================================================

std::string App::searchNearest() {
    //std::cout << strPuntsDiana << " " << puntsDiana << " " << isDianaDouble << " " << isDianaTreble << " " << isSingleBull << " " << " " << isDoubleBull<< " " << isSector1 << " " << isSector2 << std::endl;

    std::string str_all_targets = "MISS,SB,DB,20,D20,T20,1,D1,T1,18,D18,T18,4,D4,T4,13,D13,T13,6,D6,T6,10,D10,T10,15,D15,T15,2,D2,T2,17,D17,T17,3,D3,T3,19,D19,T19,7,D7,T7,16,D16,T16,8,D8,T8,11,D11,T11,14,D14,T14,9,D9,T9,12,D12,T12,5,D5,T5";

    std::string str_result = "";
    //els casos possibles de puntuació que podem tenir són els següents:
    //0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,22,24,25,26,28,30,32,34,36,38,39,40,42,45,48,50,51,54,57,60
    //considerem totes les possibilitats a força bruta
    if (puntsDiana == 0) { //MISS
        str_result = "D1,D2,D3,D4,D5,D6,D7,D8,D9,D10,D11,D12,D13,D14,D15,D16,D17,D18,D19,D20,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20";
        str_result = completar_nearest(str_result,str_all_targets,"MISS");
    } else if (puntsDiana == 1) { //1_1, 1_2, 1
        if (isSector1) { //1_1
            str_result = "18,20,D1,T1,D18,D20,T18,T20";
            str_result = completar_nearest(str_result,str_all_targets,"1");
        } else if (isSector2) { //1_2
            str_result = "18,20,T1,SB,T18,T20";
            str_result = completar_nearest(str_result,str_all_targets,"1");
        }  else { //1
            str_result = "18,20,D1,T1,SB,D18,D20,T18,T20";
            str_result = completar_nearest(str_result,str_all_targets,"1");
        }
    } else if (puntsDiana == 2) { //D1, 2_1, 2_2, 2
        if (isDianaDouble) { //D1
            str_result = "MISS,1,18,20,D18,D20";
            str_result = completar_nearest(str_result,str_all_targets,"2");
        } else if (isSector1) { //2_1
            str_result = "15,17,D2,T2,D15,D17,T15,T17";
            str_result = completar_nearest(str_result,str_all_targets,"2");
        } else if (isSector2) { //2_2
            str_result = "15,17,T2,T15,T17,SB";
            str_result = completar_nearest(str_result,str_all_targets,"2");
        }  else { //2
            str_result = "15,17,D2,T2,SB,D15,D17,T15,T17";
            str_result = completar_nearest(str_result,str_all_targets,"2");
        }
    }  else if (puntsDiana == 3) { //T1, 3_1, 3_2, 3
        if (isDianaTreble) { //T1
            str_result = "MISS,1,T18,T20,18,20";
            str_result = completar_nearest(str_result,str_all_targets,"3");
        } else if (isSector1) { //3_1
            str_result = "17,19,D3,T3,D17,D19,T17,T19";
            str_result = completar_nearest(str_result,str_all_targets,"3");
        } else if (isSector2) { //3_2
            str_result = "17,19,T3,SB,T17,T19";
            str_result = completar_nearest(str_result,str_all_targets,"3");
        }  else { //3
            str_result = "17,19,D3,T3,SB,D17,D19,T17,T19";
            str_result = completar_nearest(str_result,str_all_targets,"3");
        }
    } else if (puntsDiana == 4) { //D2, 4_1, 4_2, 4
        if (isDianaDouble) { //D2
            str_result = "MISS,2,15,17,D15,D17";
            str_result = completar_nearest(str_result,str_all_targets,"4");
        } else if (isSector1) { //4_1
            str_result = "D4,13,18,T4,D13,D18,T13,T18";
            str_result = completar_nearest(str_result,str_all_targets,"4");
        } else if (isSector2) { //4_2
            str_result = "T4,13,18,SB,T13,T18";
            str_result = completar_nearest(str_result,str_all_targets,"4");
        }  else { //4
            str_result = "13,18,D4,T4,SB,D13,D18,T13,T18";
            str_result = completar_nearest(str_result,str_all_targets,"4");
        }
    } else if (puntsDiana == 5) { //5_1, 5_2, 5
        if (isSector1) { //5_1
            str_result = "MISS,20,12,D5,D20,D12,T5,T20,T12";
            str_result = completar_nearest(str_result,str_all_targets,"5");
        } else if (isSector2) { //5_2
            str_result = "20,12,T5,T20,T12,SB";
            str_result = completar_nearest(str_result,str_all_targets,"5");
        }  else { //5
            str_result = "20,12,D5,D20,D12,T5,T20,T12,SB";
            str_result = completar_nearest(str_result,str_all_targets,"5");
        }
    }  else if (puntsDiana == 6) { //D3, T2, 6_1, 6_2, 6
        if (isDianaDouble) { //D3
            str_result = "MISS,3,D17,D19,17,19";
            str_result = completar_nearest(str_result,str_all_targets,"6");
        } else if (isDianaTreble) { //T2
            str_result = "2,T15,T17,15,17";
            str_result = completar_nearest(str_result,str_all_targets,"6");
        } else if (isSector1) { //6_1
            str_result = "10,13,D6,T6,D10,D13,T10,T13";
            str_result = completar_nearest(str_result,str_all_targets,"6");
        } else if (isSector2) { //6_2
            str_result = "10,13,T6,SB,T10,T13";
            str_result = completar_nearest(str_result,str_all_targets,"6");
        }  else { //6
            str_result = "10,13,D6,T6,SB,D10,D13,T10,T13";
            str_result = completar_nearest(str_result,str_all_targets,"6");
        }
    }  else if (puntsDiana == 7) { //7_1, 7_2, 7
        if (isSector1) { //7_1
            str_result = "16,19,D7,T7,D16,D19,T16,T19";
            str_result = completar_nearest(str_result,str_all_targets,"7");
        } else if (isSector2) { //7_2
            str_result = "16,19,T7,SB,T16,T19";
            str_result = completar_nearest(str_result,str_all_targets,"7");
        }  else { //7
            str_result = "16,19,D7,T7,SB,D16,D19,T16,T19";
            str_result = completar_nearest(str_result,str_all_targets,"7");
        }
    }  else if (puntsDiana == 8) { //D4, 8_1, 8_2, 8
        if (isDianaDouble) { //D4
            str_result = "MISS,4,D13,D18,13,18";
            str_result = completar_nearest(str_result,str_all_targets,"8");
        } else if (isSector1) { //8_1
            str_result = "11,16,D8,T8,D11,D16,T11,T16";
            str_result = completar_nearest(str_result,str_all_targets,"8");
        } else if (isSector2) { //8_2
            str_result = "11,16,T8,SB,T11,T16";
            str_result = completar_nearest(str_result,str_all_targets,"8");
        }  else { //8
            str_result = "11,16,D8,T8,D11,D16,T11,T16,SB";
            str_result = completar_nearest(str_result,str_all_targets,"8");
        }
    }  else if (puntsDiana == 9) { //T3, 9_1, 9_2, 9
        if (isDianaTreble) { //T3
            str_result = "3,17,19,T17,T19";
            str_result = completar_nearest(str_result,str_all_targets,"9");
        } else if (isSector1) { //9_1
            str_result = "12,14,D9,T9,D12,D14,T12,T14";
            str_result = completar_nearest(str_result,str_all_targets,"9");
        } else if (isSector2) { //9_2
            str_result = "12,14,T9,SB,T12,T14";
            str_result = completar_nearest(str_result,str_all_targets,"9");
        }  else { //9
            str_result = "12,14,D9,T9,SB,D12,D14,T12,T14";
            str_result = completar_nearest(str_result,str_all_targets,"9");
        }
    }  else if (puntsDiana == 10) { //D5, 10_1, 10_2, 10
        if (isDianaDouble) { //D5
            str_result = "MISS,5,D12,D20,12,20";
            str_result = completar_nearest(str_result,str_all_targets,"10");
        } else if (isSector1) { //10_1
            str_result = "6,15,D10,T10,D6,D15,T6,T15";
            str_result = completar_nearest(str_result,str_all_targets,"10");
        } else if (isSector2) { //10_2
            str_result = "6,15,T10,SB,T6,T15";
            str_result = completar_nearest(str_result,str_all_targets,"10");
        }  else { //10
            str_result = "6,15,D10,T10,SB,D6,D15,T6,T15";
            str_result = completar_nearest(str_result,str_all_targets,"10");
        }
    }  else if (puntsDiana == 11) { //11_1, 11_2, 11
        if (isSector1) { //11_1
            str_result = "8,14,D11,T11,D8,D14,T8,T14";
            str_result = completar_nearest(str_result,str_all_targets,"11");
        } else if (isSector2) { //11_2
            str_result = "8,14,T11,SB,T8,T14";
            str_result = completar_nearest(str_result,str_all_targets,"11");
        }  else { //11
            str_result = "8,14,D11,T11,SB,D8,D14,T8,T14";
            str_result = completar_nearest(str_result,str_all_targets,"11");
        }
    }  else if (puntsDiana == 12) { //D6, T4, 6_1, 6_2, 6
        if (isDianaDouble) { //D6
            str_result = "MISS,6,D10,D13,10,13";
            str_result = completar_nearest(str_result,str_all_targets,"12");
        } else if (isDianaTreble) { //T4
            str_result = "4,T13,T18,13,18";
            str_result = completar_nearest(str_result,str_all_targets,"12");
        } else if (isSector1) { //12_1
            str_result = "5,9,D12,T12,D5,D9,T5,T9";
            str_result = completar_nearest(str_result,str_all_targets,"12");
        } else if (isSector2) { //12_2
            str_result = "5,9,T12,SB,T5,T9";
            str_result = completar_nearest(str_result,str_all_targets,"12");
        }  else { //12
            str_result = "5,9,D12,T12,SB,D5,D9,T5,T9";
            str_result = completar_nearest(str_result,str_all_targets,"12");
        }
    }  else if (puntsDiana == 13) { //13_1, 13_2, 13
        if (isSector1) { //13_1
            str_result = "4,6,D13,T13,D4,D6,T4,T6";
            str_result = completar_nearest(str_result,str_all_targets,"13");
        } else if (isSector2) { //13_2
            str_result = "4,6,T13,SB,T4,T6";
            str_result = completar_nearest(str_result,str_all_targets,"13");
        }  else { //13
            str_result = "4,6,D13,T13,SB,D4,D6,T4,T6";
            str_result = completar_nearest(str_result,str_all_targets,"13");
        }
    }  else if (puntsDiana == 14) { //D7, 14_1, 14_2, 14
        if (isDianaDouble) { //D7
            str_result = "MISS,7,D16,D19,16,19";
            str_result = completar_nearest(str_result,str_all_targets,"14");
        } else if (isSector1) { //14_1
            str_result = "9,11,D14,T14,D9,D11,T9,T11";
            str_result = completar_nearest(str_result,str_all_targets,"14");
        } else if (isSector2) { //14_2
            str_result = "9,11,T14,SB,T9,T11";
            str_result = completar_nearest(str_result,str_all_targets,"14");
        }  else { //14
            str_result = "9,11,D14,T14,SB,D9,D11,T9,T11";
            str_result = completar_nearest(str_result,str_all_targets,"14");
        }
    }  else if (puntsDiana == 15) { //T5, 15_1, 15_2, 15
        if (isDianaDouble) { //T5
            str_result = "5,T12,T20,12,20";
            str_result = completar_nearest(str_result,str_all_targets,"15");
        } else if (isSector1) { //15_1
            str_result = "2,10,D15,T15,D2,D10,T2,T10";
            str_result = completar_nearest(str_result,str_all_targets,"15");
        } else if (isSector2) { //15_2
            str_result = "2,10,T15,SB,T2,T10";
            str_result = completar_nearest(str_result,str_all_targets,"15");
        }  else { //15
            str_result = "2,10,D15,T15,SB,D2,D10,T2,T10";
            str_result = completar_nearest(str_result,str_all_targets,"15");
        }
    }  else if (puntsDiana == 16) { //D8, 16_1, 16_2, 16
        if (isDianaDouble) { //D8
            str_result = "MISS,8,D11,D16,11,16";
            str_result = completar_nearest(str_result,str_all_targets,"16");
        } else if (isSector1) { //16_1
            str_result = "7,8,D16,T16,D7,D8,T7,T8";
            str_result = completar_nearest(str_result,str_all_targets,"16");
        } else if (isSector2) { //16_2
            str_result = "7,8,T16,SB,T7,T8";
            str_result = completar_nearest(str_result,str_all_targets,"16");
        }  else { //16
            str_result = "7,8,D16,T16,SB,D7,D8,T7,T8";
            str_result = completar_nearest(str_result,str_all_targets,"16");
        }
    }  else if (puntsDiana == 17) { //17_1, 17_2, 17
        if (isSector1) { //17_1
            str_result = "2,3,D17,T17,D2,D3,T2,T3";
            str_result = completar_nearest(str_result,str_all_targets,"17");
        } else if (isSector2) { //17_2
            str_result = "2,3,T17,SB,T2,T3";
            str_result = completar_nearest(str_result,str_all_targets,"17");
        }  else { //17
            str_result = "2,3,D17,T17,SB,D2,D3,T2,T3";
            str_result = completar_nearest(str_result,str_all_targets,"17");
        }
    }  else if (puntsDiana == 18) { //D9, T6, 18_1, 18_2, 18
        if (isDianaDouble) { //D9
            str_result = "MISS,9,D12,D14,12,14";
            str_result = completar_nearest(str_result,str_all_targets,"18");
        } else if (isDianaTreble) { //T6
            str_result = "6,T10,T13,10,13";
            str_result = completar_nearest(str_result,str_all_targets,"18");
        } else if (isSector1) { //18_1
            str_result = "1,4,D18,T18,D1,D4,T1,T4";
            str_result = completar_nearest(str_result,str_all_targets,"18");
        } else if (isSector2) { //18_2
            str_result = "1,4,T18,SB,T1,T4";
            str_result = completar_nearest(str_result,str_all_targets,"18");
        }  else { //18
            str_result = "1,4,D18,T18,SB,D1,D4,T1,T4";
            str_result = completar_nearest(str_result,str_all_targets,"18");
        }
    }  else if (puntsDiana == 19) { //19_1, 19_2, 19
        if (isSector1) { //19_1
            str_result = "3,7,D19,T19,D3,D7,T3,T7";
            str_result = completar_nearest(str_result,str_all_targets,"19");
        } else if (isSector2) { //19_2
            str_result = "3,7,T19,SB,T3,T7";
            str_result = completar_nearest(str_result,str_all_targets,"19");
        }  else { //19
            str_result = "3,7,D19,T19,SB,D3,D7,T3,T7";
            str_result = completar_nearest(str_result,str_all_targets,"19");
        }
    } else if (puntsDiana == 20) { //D10, 20_1, 20_2, 20
        if (isDianaDouble) { //D10
            str_result = "MISS,10,D6,D15,6,15";
            str_result = completar_nearest(str_result,str_all_targets,"20");
        } else if (isSector1) { //20_1
            str_result = "1,5,D20,T20,D1,D5,T1,T5";
            str_result = completar_nearest(str_result,str_all_targets,"20");
        } else if (isSector2) { //20_2
            str_result = "1,5,T20,SB,T1,T5";
            str_result = completar_nearest(str_result,str_all_targets,"20");
        } else { //20
            str_result = "1,5,D20,T20,SB,D1,D5,T1,T5";
            str_result = completar_nearest(str_result,str_all_targets,"20");
        }
    } else if (puntsDiana == 21) { //T7
        str_result = "7,T16,T19,16,19";
        str_result = completar_nearest(str_result,str_all_targets,"21");
    }  else if (puntsDiana == 22) { //D11
        str_result = "MISS,11,D8,D14,8,14";
        str_result = completar_nearest(str_result,str_all_targets,"22");
    }  else if (puntsDiana == 24) { //D12,T8
        if (isDianaDouble) { //D12
            str_result = "MISS,12,D5,D9,5,9";
            str_result = completar_nearest(str_result,str_all_targets,"24");
        } else if (isDianaTreble) { //T8
            str_result = "8,T11,T16,11,16";
            str_result = completar_nearest(str_result,str_all_targets,"24");
        }
    }  else if (puntsDiana == 25) { //SB
        str_result = "DB,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20";
        str_result = completar_nearest(str_result,str_all_targets,"25");
    }  else if (puntsDiana == 26) { //D13
        str_result = "MISS,13,D4,D6,4,6";
        str_result = completar_nearest(str_result,str_all_targets,"26");
    }  else if (puntsDiana == 28) { //D14
        str_result = "MISS,14,D9,D11,9,11";
        str_result = completar_nearest(str_result,str_all_targets,"28");
    }  else if (puntsDiana == 30) { //D15, T10
        if (isDianaDouble) { //D15
            str_result = "MISS,15,D2,D10,2,10";
            str_result = completar_nearest(str_result,str_all_targets,"30");
        } else if (isDianaTreble) { //T10
            str_result = "10,T6,15,6,15";
            str_result = completar_nearest(str_result,str_all_targets,"30");
        }
    }  else if (puntsDiana == 32) { //D16
        str_result = "MISS,16,D7,D8,7,8";
        str_result = completar_nearest(str_result,str_all_targets,"32");
    }  else if (puntsDiana == 34) { //D17
        str_result = "MISS,17,D2,D3,2,3";
        str_result = completar_nearest(str_result,str_all_targets,"34");
    }  else if (puntsDiana == 36) { //D18, T12
        if (isDianaDouble) { //D18
            str_result = "MISS,18,D1,D4,1,4";
            str_result = completar_nearest(str_result,str_all_targets,"36");
        } else if (isDianaTreble) { //T12
            str_result = "12,T5,T9,5,9";
            str_result = completar_nearest(str_result,str_all_targets,"36");
        }
    }  else if (puntsDiana == 38) { //D19
        str_result = "MISS,19,D3,D7,3,7";
        str_result = completar_nearest(str_result,str_all_targets,"38");
    }  else if (puntsDiana == 40) { //D20
        str_result = "MISS,20,D1,D5,1,5";
        str_result = completar_nearest(str_result,str_all_targets,"40");
    }  else if (puntsDiana == 42) { //T14
        str_result = "14,T9,T11,9,11";
        str_result = completar_nearest(str_result,str_all_targets,"42");
    }  else if (puntsDiana == 45) { //T15
        str_result = "15,T2,T10,2,10";
        str_result = completar_nearest(str_result,str_all_targets,"45");
    }  else if (puntsDiana == 48) { //T16
        str_result = "16,T7,T8,7,8";
        str_result = completar_nearest(str_result,str_all_targets,"48");
    }  else if (puntsDiana == 50) { //DB
        str_result = "SB,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20";
        str_result = completar_nearest(str_result,str_all_targets,"50");
    }  else if (puntsDiana == 51) { //T17
        str_result = "17,T2,T3,2,3";
        str_result = completar_nearest(str_result,str_all_targets,"51");
    }  else if (puntsDiana == 54) { //T18
        str_result = "18,T1,T4,1,4";
        str_result = completar_nearest(str_result,str_all_targets,"54");
    }  else if (puntsDiana == 57) { //T19
        str_result = "19,T3,T7,3,7";
        str_result = completar_nearest(str_result,str_all_targets,"57");
    }  else if (puntsDiana == 60) { //T20
        str_result = "20,T1,T5,1,5";
        str_result = completar_nearest(str_result,str_all_targets,"60");
    }  else {
        str_result = "not implemented";
    }

    return str_result;
}

std::string completar_nearest(std::string str1, std::string str2, std::string str3) {
    std::vector<std::string> elems1, elems2;
    elems1 = split(str1, ',');
    elems2 = split(str2, ',');
    str2 = "";
    int found;

    for(std::vector<std::string>::size_type i = 0; i != elems2.size(); i++) {
        found = 0;
        for(std::vector<std::string>::size_type j = 0; j != elems1.size(); j++) {
            if (elems1.at(j) == elems2.at(i) || str3 == elems2.at(i)) found = 1;
        }
        if (!found) str2 += elems2.at(i) + ",";
    }

    str2 = str2.substr(0, str2.size()-1);
    if (str2 == "") {
        return str1;  
    } else {
        return str1 + "," + str2;        
    }

}
