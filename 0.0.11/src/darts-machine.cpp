/*
cd /home/joan/projectes/darts-machine/0.0.11/src
PS1="$ "
*/
// g++ -Wall -std=c++11 -o darts-machine darts-machine.cpp App.o globalFunctions.o globalVars.o Screen.o ScreenElement.o Game.o Player.o -lrt
// g++ -Wall -std=c++0x -o darts-machine darts-machine.cpp App.o globalFunctions.o globalVars.o Screen.o ScreenElement.o Game.o Player.o -lrt
// o bé
// g++ -Wall -std=c++11 -o darts-machine darts-machine.cpp App.cpp globalFunctions.cpp globalVars.cpp Screen.cpp ScreenElement.cpp Game.cpp Player.cpp -lrt
// g++ -Wall -std=c++0x -o darts-machine darts-machine.cpp App.cpp globalFunctions.cpp globalVars.cpp Screen.cpp ScreenElement.cpp Game.cpp Player.cpp -lrt

//warning: range-based ‘for’ loops only available with -std=c++11 or -std=gnu++11 (or -std=c++0x)
// (App.cpp) Per tal de poder recórrer els vectors amb la notació abreviada for(Screen sc : pantalles2) , en comptes d'utilitzar for (unsigned int i=0; i<pantalles2.size(); i++) {
// i accedir als elements amb .at(i), he de compilar amb -std=c++11

// per controlar la memòria que consumeix l'aplicatiu:
// $ top -bc |grep darts-machine

#include <cstdlib> 
#include "App.h"

char SOFTWARE[] = "darts-machine";
char DATE[] = "gener";
char YEAR[] = "2018";
char AUTHOR[] = "Joan Quintana Compte-joanillo";
char LICENSE[] = "GPL v.3";
char WEB[] = "joanqc@gmail.com - www.retroplaneta.com - www.joanillo.org";
char VERSION[] = "v0.0.11";

int main (int argc, char ** argv){

    std::cout << std::string(20, '\n'); //en comptes de system("clear")
	std::cout << SOFTWARE << " " << VERSION << ". " << AUTHOR << std::endl << std::endl;

	//creo una nova aplicació
	//construim std::cout << "destruir-ho tot i shutdown" << std::endl;totes les pantalles i elements
	App nova_app("new_app");
	//std::cout << nova_app.getName() << std::endl;
	//std::cout << "**** " << nova_app.pantalles[0].getName() << std::endl;
	//i entrem en el loop...
	nova_app.loopGame();
	
	//shutdown
	std::cout << "FER SHUTDOWN" << std::endl;

	//sortim
	return 0;
}

