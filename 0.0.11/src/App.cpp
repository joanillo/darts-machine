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
void updateVarsScore();

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
        stroptionID = stroptionID.substr (1,stroptionID.length()-1); 
    
        if (stroptionIDType == "s") { //score
            strPuntsDiana = stroptionID;
            updateVarsScore();
            if (numDart <= 3 && gameOver == false) App::logica(); //si estem en el 4rt dart, obligatòriament hem de pitjar/seleccionar Change player/turn. Tampoc puc introduir puntuació si ja hem acabat.
        } else if (stroptionIDType == "u") { //user interface
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
            } else if (optionName == "Change player/turn") {
                std::cout << "Change player/turn" << std::endl;
                App::changePlayerTurn();
            } else if (optionName == "Modify score") {
                std::cout << "MODIFY SCORE" << std::endl;

            std::cout << "tornJugador: " << tornJugadorPrev << " " << tornJugador << std::endl;
            std::cout << "numRonda: " << numRondaPrev << " " << numRonda << std::endl;


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
        std::cout << puntsJugadorCricketPrev[tornJugador-1][j-1] << std::endl;
        puntsJugadorCricket[tornJugador-1][j-1] = puntsJugadorCricketPrev[tornJugador-1][j-1];
        puntsJugadorCricketSigne[tornJugador-1][j-1] = puntsJugadorCricketSignePrev[tornJugador-1][j-1];
    }

    //Count Up

    //Halve It

    App::updateScreenplay();
}

            } 

        } else {
            std::cout << "Opció no correcta" << std::endl;
        }

    }
}

// =============================================================================================

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
    std::cout << scr_ele.getName() << ": "<< numRonda << std::endl;

    //tornJugador
    scr_ele = pantalla.getScreenElementfromName("txt_tornJugador");
    std::cout << scr_ele.getName() << ": "<< tornJugador << std::endl;

    //puntsDiana
    scr_ele = pantalla.getScreenElementfromName("txt_puntsDiana");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << puntsDiana << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << puntsGuanyador << std::endl;
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
                std::cout << scr_ele.getName() << ": " << puntsJugadorDart[i-1][j] << std::endl;
            }
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << puntsJugadorTotal[i-1] << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if (isBusts == true) {
        std::cout << scr_ele.getName() << ": "<< "BUSTS" << std::endl;
    } else if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "SB") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "DB") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ": " << strPuntsDiana << std::endl;
        }
    }  else if (gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "25") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "50") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ":" << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< "END GAME. WINNER:" << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << partida.jugador.at(guanyador).getAvatar() << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << ". Prefix 's' for score: (sXX, sXXD, sXXT, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/turn
    scr_ele = pantalla.getScreenElementfromName("Change player/turn");
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
        std::cout << scr_ele.getName() << ": " << puntsDiana << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << puntsGuanyador << std::endl;
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
                std::cout << scr_ele.getName() << ": " << puntsJugadorDart[i-1][j] << std::endl;
            }
        }

        for (unsigned int j=0;j<sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);j++) { //recorrem les 8 rondes
            //puntsJugadorRonda txt_puntsRonda4Jugador" + int_to_string(i)
            //scr_ele = pantalla.getScreenElementfromName("txt_puntsDart" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            scr_ele = pantalla.getScreenElementfromName("txt_puntsRonda" + int_to_string(j+1) + "Jugador" + int_to_string(i));
            std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << puntsJugadorRonda[i-1][j] << std::endl;
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << puntsJugadorTotal[i-1] << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "SB") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "DB") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ": " << strPuntsDiana << std::endl;
        }
    } else if (gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "25") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "50") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ":" << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< "END GAME. WINNER:" << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << partida.jugador.at(guanyador).getAvatar() << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << ". Prefix 's' for score: (sXX, sXXD, sXXT, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/turn
    scr_ele = pantalla.getScreenElementfromName("Change player/turn");
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
        std::cout << scr_ele.getName() << ": " << puntsDiana << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << puntsGuanyador << std::endl;
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
                std::cout << scr_ele.getName() << ": " << puntsJugadorDart[i-1][j] << std::endl;
            }
        }

        for (unsigned int j=1;j<=sizeof(cricketTargets)/sizeof(cricketTargets[0]);j++) { //recorrem els targets del Cricket
            //puntsJugadorRonda
            scr_ele = pantalla.getScreenElementfromName("txt_puntsCricket" + int_to_string(j) + "Jugador" + int_to_string(i));
            std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << puntsJugadorCricket[i-1][j-1] << " " << puntsJugadorCricketSigne[i-1][j-1] << std::endl;
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << puntsJugadorTotal[i-1] << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ( (strPuntsDiana == "MISS" || strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "SB") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "DB") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ": " << strPuntsDiana << std::endl;
        }
    } else if (gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "25") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "50") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ":" << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< "END GAME. WINNER:" << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << partida.jugador.at(guanyador).getAvatar() << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << ". Prefix 's' for score: (sXX, sXXD, sXXT, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/turn
    scr_ele = pantalla.getScreenElementfromName("Change player/turn");
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
        std::cout << scr_ele.getName() << ": " << puntsDiana << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << puntsGuanyador << std::endl;
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
                std::cout << scr_ele.getName() << ": " << puntsJugadorDart[i-1][j] << std::endl;
            }
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << roundtheclockTargets[puntsJugadorRoundTheClock[i-1]] << std::endl; 
    }

    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "SB") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "DB") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ": " << strPuntsDiana << std::endl;
        }
    }  else if (gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "25") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "50") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ":" << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< "END GAME. WINNER:" << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << partida.jugador.at(guanyador).getAvatar() << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << ". Prefix 's' for score: (sXX, sXXD, sXXT, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/turn
    scr_ele = pantalla.getScreenElementfromName("Change player/turn");
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
        std::cout << scr_ele.getName() << ": " << puntsDiana << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << puntsGuanyador << std::endl;
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
                std::cout << scr_ele.getName() << ": " << puntsJugadorDart[i-1][j] << std::endl;
            }
        }

        for (unsigned int j=1;j<=sizeof(halveitTargets)/sizeof(halveitTargets[0]);j++) { //recorrem els targets del Cricket
            //puntsJugadorRonda
            scr_ele = pantalla.getScreenElementfromName("txt_puntsHalveIt" + int_to_string(j) + "Jugador" + int_to_string(i));
            //std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << puntsJugadorRonda[i-1][j-1] << std::endl;
            std::cout << scr_ele.getName() << ": " << scr_ele.getName2() << " - " << puntsJugadorHalveIt[i-1][j-1] << std::endl;
        }

        //puntsJugadorxTotal
        scr_ele = pantalla.getScreenElementfromName("txt_puntsTotalJugador" + int_to_string(i));
        std::cout << scr_ele.getName() << ": " << puntsJugadorTotal[i-1] << std::endl;   
    }
    
    //text final de partida
    scr_ele = pantalla.getScreenElementfromName("txt_textInfo");
    if ((strPuntsDiana.find("D") != std::string::npos || strPuntsDiana.find("T") != std::string::npos || strPuntsDiana.find("SB") != std::string::npos || strPuntsDiana.find("DB") != std::string::npos) && gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "SB") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "DB") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ": " << strPuntsDiana << std::endl;
        }
    } else if (gameOver == false) {
        if (strPuntsDiana == "MISS") {
            std::cout << scr_ele.getName() << ": " << "MISS" << std::endl;
        } else if (strPuntsDiana == "25") {
            std::cout << scr_ele.getName() << ": " << "Single-Bull" << std::endl;
        } else if (strPuntsDiana == "50") {
            std::cout << scr_ele.getName() << ": " << "Double-Bull" << std::endl;
        } else {
            std::cout << scr_ele.getName() << ":" << std::endl;
        }
    } else {
        std::cout << scr_ele.getName() << ": "<< "END GAME. WINNER:" << std::endl;
    }

    //Guanyador
    scr_ele = pantalla.getScreenElementfromName("txt_guanyador");
    if (gameOver == false) {
        std::cout << scr_ele.getName() << ": " << std::endl;
    } else {
        std::cout << scr_ele.getName() << ": " << partida.jugador.at(guanyador).getAvatar() << std::endl;
    }

    //Introduir punts diana
    scr_ele = pantalla.getScreenElementfromName("User interaction");
    std::cout << std::endl << scr_ele.getName() << ". Prefix 's' for score: (sXX, sXXD, sXXT, sSB, sDB, s0): prefix 'u' for interface" << std::endl;

    //Change player/turn
    scr_ele = pantalla.getScreenElementfromName("Change player/turn");
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
    isBusts = false;
    isMiss = true;

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

    for (unsigned int i=0;i<sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);i++) { //num_rondes
        for (unsigned int j=0;j<gameNumPlayers;j++) { //num_jugadors
            puntsJugadorRonda[j][i] = 0;
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

void updateVarsScore() {

    std::string strTemp;
    strTemp = strPuntsDiana;

    if (strPuntsDiana == "SB") {
        puntsDiana = 25;
        isSingleBull = true;
        isDoubleBull = false;
        isDianaDouble = false;
        isDianaTreble = false;
        isMiss = false;
    }   else if (strPuntsDiana == "DB") {
        puntsDiana = 50;
        isSingleBull = false;
        isDoubleBull = true;
        isDianaDouble = false;
        isDianaTreble = false;
        isMiss = false;
    } else if (strPuntsDiana.find("D") != std::string::npos) {
        replace (strTemp, "D", "");
        puntsDiana = atoi(strTemp.c_str())*2;
        isSingleBull = false;
        isDoubleBull = false;
        isDianaDouble = true;
        isDianaTreble = false;
        isMiss = false;
    } else if (strPuntsDiana.find("T") != std::string::npos) {
        replace (strTemp, "T", "");
        puntsDiana = atoi(strTemp.c_str())*3;
        isSingleBull = false;
        isDoubleBull = false;
        isDianaDouble = false;
        isDianaTreble = true;
        isMiss = false;
    } else {
        puntsDiana = atoi(strPuntsDiana.c_str());
        if (puntsDiana == 25) { isSingleBull = true;isDoubleBull = false;}
        if (puntsDiana == 50) { isDoubleBull = true;isSingleBull = false;}
        if (puntsDiana == 0) isMiss = true; else isMiss = false; 
        isDianaDouble = false;
        isDianaTreble = false;
    }

    if (isMiss) strPuntsDiana = "MISS";
}

// =============================================================================================

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
    puntsJugadorRonda[tornJugador-1][numRonda-1] += puntsDiana;
    puntsJugadorTotal[tornJugador-1] += puntsDiana;

    App::updateScreenplay_Count_Up();

    numDart++;
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

    App::updateScreenplay();
    numDart++;
}

// =============================================================================================
void App::changePlayerTurn() {
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
            sleep(1); //espera per veure els resultats, abans d'anar al següent jugador (quant ingui un botó ja no caldrà)
            isBusts = false;
        } else {

            tornJugadorPrev = tornJugador;
            numRondaPrev = numRonda;

            tornJugador = 1;
            numRonda++;
            std::cout << "WAIT..." << std::endl;
            sleep(1); //espera per veure els resultats, abans d'anar a la següent ronda (quant ingui un botó ja no caldrà)
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
