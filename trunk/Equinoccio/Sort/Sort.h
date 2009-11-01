#ifndef __SORT_H_INCLUDED__
#define __SORT_H_INCLUDED__

#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>

class Sorter{
private:
     Sorter(){};
public:
     static uint32_t Sort(const std::string &nombreArchivo,			\
			  const std::string &nombreBaseSalida, uint32_t primero, \
			  unsigned cantMax);
};

#endif //__SORT_H_INCLUDED__
