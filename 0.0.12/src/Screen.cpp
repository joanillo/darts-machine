/* g++ -Wall -std=c++0x -c Screen.cpp */

//pantallaelements: en comptes d'un array ho canvio a vector

#include "Screen.h"
#include <vector>

// Constructor
Screen::Screen(unsigned int ID, std::string name, int numElements) {
	this->ID = ID;
	this->name = name;
	this->numElements = numElements;
}

unsigned int Screen::getID() const {
	return ID;
}

void Screen::setID(unsigned int ID) {
	this->ID = ID;
}

std::string Screen::getName() const {
	return name;
}
 
void Screen::setName(std::string name) {
	this->name = name;
}

void Screen::createScreenElements(std::string screenName) {
	//std::cout << "** " << screenName << std::endl;
	ScreenElement ScreenElementTemp;

	unsigned int id = 0;

	pantallaelements.clear();

	//els elements de les pantalles estaran parametritzats en un fitxer.
	if (screenName == "new_game") { //**************************************************************************************************
		id = 0;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Choose Game");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		std::fstream myfile;
		std::string line;

		myfile.open ("../resources/games.txt");

		if (myfile.is_open())
		{
		while ( getline (myfile,line) )
		{
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName(line);
			ScreenElementTemp.setClickable(1);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}
		myfile.close();
		}
		else 	std::cout << "Unable to open file" << std::endl;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Info");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	} else if (screenName == "num_players") { //**************************************************************************************************
		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Choose Number of Players");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("1 player");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("2 players");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("3 players");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("4 players");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	} else if (screenName == "assign_avatars") { //**************************************************************************************************

		id = 0;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Assign Avatar");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		std::fstream myfile;
		std::string line;

		myfile.open ("../resources/avatars.txt");

		if (myfile.is_open())
		{
		while ( getline (myfile,line) )
		{
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName(line);
			ScreenElementTemp.setClickable(1);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName(replace(line," ","_") + ".png");
			ScreenElementTemp.setClickable(0);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}
		myfile.close();
		}
		else 	std::cout << "Unable to open file" << std::endl;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	} else if (screenName == "show_app_info") { //**************************************************************************************************

		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("DARTS INFO");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Regles");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		std::fstream myfile;
		std::string line;

		myfile.open ("../resources/games.txt");

		if (myfile.is_open())
		{
		while ( getline (myfile,line) )
		{
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("r_"+ line);
			ScreenElementTemp.setClickable(1);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}
		myfile.close();
		}
		else 	std::cout << "Unable to open file" << std::endl;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Stats Game");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		myfile.open ("../resources/games.txt");

		if (myfile.is_open())
		{
		while ( getline (myfile,line) )
		{
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("sg_"+ line);
			ScreenElementTemp.setClickable(1);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}
		myfile.close();
		}
		else 	std::cout << "Unable to open file" << std::endl;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Stats Player");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		myfile.open ("../resources/avatars.txt");

		if (myfile.is_open())
		{
		while ( getline (myfile,line) )
		{
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("sp_"+ line);
			ScreenElementTemp.setClickable(1);
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}
		myfile.close();
		}
		else 	std::cout << "Unable to open file" << std::endl;


		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	} else if (screenName == "game_301" || screenName == "game_501" || screenName == "game_301D" || screenName == "game_501D" || screenName == "game_Round the Clock") { //**************************************************************************************************
		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName(replace(screenName,"game_",""));

		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_numRonda");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_tornJugador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_puntsDiana");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		for (unsigned int i=1;i<=gameNumPlayers;i++) {
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_picjugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_jugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			for (int j=1;j<=3;j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsDart" + int_to_string(j) + "Jugador" + int_to_string(i));
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_puntsTotalJugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_textInfo");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_guanyador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Change player/round");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Modify score");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;
		
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	}  else if (screenName == "game_Count Up") { //**************************************************************************************************
		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Count Up");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_numRonda");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_tornJugador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_puntsDiana");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		for (unsigned int i=1;i<=gameNumPlayers;i++) {
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_picjugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_jugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			for (int j=1;j<=3;j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsDart" + int_to_string(j) + "Jugador" + int_to_string(i));
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			for (unsigned int j=1;j<=sizeof(puntsJugadorRonda[0])/sizeof(puntsJugadorRonda[0][0]);j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsRonda" + int_to_string(j) + "Jugador" + int_to_string(i));
				ScreenElementTemp.setName2(int_to_string(j));				
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_puntsTotalJugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_textInfo");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_guanyador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Change player/round");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Modify score");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	}  else if (screenName == "game_Cricket") { //**************************************************************************************************
		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Cricket");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_numRonda");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_tornJugador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_puntsDiana");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		for (unsigned int i=1;i<=gameNumPlayers;i++) {
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_picjugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_jugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			for (int j=1;j<=3;j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsDart" + int_to_string(j) + "Jugador" + int_to_string(i));
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			for (unsigned int j=1;j<=sizeof(cricketTargets)/sizeof(cricketTargets[0]);j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsCricket" + int_to_string(j) + "Jugador" + int_to_string(i));
				ScreenElementTemp.setName2(cricketTargets[j-1]);					
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_puntsTotalJugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_textInfo");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_guanyador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Change player/round");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Modify score");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

	}  else if (screenName == "game_Halve It") { //**************************************************************************************************
		id = 0;
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Halve It");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_numRonda");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_tornJugador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_puntsDiana");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		for (unsigned int i=1;i<=gameNumPlayers;i++) {
			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_picjugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_jugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;

			for (int j=1;j<=3;j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsDart" + int_to_string(j) + "Jugador" + int_to_string(i));
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			for (unsigned int j=1;j<=sizeof(halveitTargets)/sizeof(halveitTargets[0]);j++) {
				ScreenElementTemp.setID(id);
				ScreenElementTemp.setName("txt_puntsHalveIt" + int_to_string(j) + "Jugador" + int_to_string(i));
				ScreenElementTemp.setName2(halveitTargets[j-1]);					
				pantallaelements.push_back(ScreenElementTemp);
				id++;
			}

			ScreenElementTemp.setID(id);
			ScreenElementTemp.setName("txt_puntsTotalJugador" + int_to_string(i));
			pantallaelements.push_back(ScreenElementTemp);
			id++;
		}

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_textInfo");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("txt_guanyador");
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Change player/round");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;
		
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Modify score");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;
		
		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Exit");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Shutdown");
		ScreenElementTemp.setClickable(1);
		pantallaelements.push_back(ScreenElementTemp);
		id++;

		ScreenElementTemp.setID(id);
		ScreenElementTemp.setName("Retroplaneta.com");
		ScreenElementTemp.setClickable(0);
		pantallaelements.push_back(ScreenElementTemp);
		id++;
		
	}
}


unsigned int Screen::getScreenElementIDfromName(std::string elementName) const {
	unsigned int id;
	for(ScreenElement screle : pantallaelements) {
		if (screle.getName() == elementName) {
			id = screle.getID();
			break;
		}
    }
	return id;
}

ScreenElement Screen::getScreenElementfromName(std::string elementName) const {
	ScreenElement screenelement;
	for(ScreenElement screle : pantallaelements) {
		if (screle.getName() == elementName) {
			screenelement = screle;
			break;
		}
    }
	return screenelement;
}