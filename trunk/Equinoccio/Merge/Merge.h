#ifndef __MERGE_H_INCLUDED__
#define __MERGE_H_INCLUDED__
#include "../Arbol/RedBlackTree.h"
#include "../Registros/Registro.h"
#include "../Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Clase encargada de realizar el merge mediante Optimal Merge.
 **/ 
class Merger{
private:
     Merger(){};
public:

	/**
	 * 
	 * Funcion encargada de realizar el merge de las particiones
	 * utilizando para ello el metodo de Optimal Merge
	 * 
	 * @param nombreParticiones Es un vector con los nombres de las particiones.
	 * 		  nombreSalida Es el nombre del archivo que se obtiene luego del merge.
	 *
	 **/  
     static int Merge(const std::vector<std::string>& nombreParticiones, \
		      const std::string& nombreSalida);
     
};

#endif //__MERGE_H_INCLUDED__
