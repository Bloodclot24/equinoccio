#ifndef __MERGE_H_INCLUDED__
#define __MERGE_H_INCLUDED__
#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Merger{
private:
     Merger(){};
public:
     static int Merge(const std::vector<std::string>& nombreParticiones, \
		      const std::string& nombreSalida);
     
};

#endif //__MERGE_H_INCLUDED__
