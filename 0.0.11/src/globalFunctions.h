#ifndef GLOBALFUNCTIONS_H
#define GLOBALFUNCTIONS_H

//standard C++ lib
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib> //system
#include <fstream>
#include <ctime>
//#include <math.h>


//standard C lib
#include <stdlib.h> //atoi

std::string& replace(std::string& , const std::string& , const std::string& );
std::vector<std::string> &split(const std::string &, char, std::vector<std::string> &);
std::vector<std::string> split(const std::string &, char);
timespec diff(timespec start, timespec end);
std::string int_to_string(unsigned int);

#endif /* GLOBALFUNCTIONS_H */
