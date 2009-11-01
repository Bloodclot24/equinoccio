#ifndef __SORT_H_INCLUDED__
#define __SORT_H_INCLUDED__

#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>


/**
 * Clase que se encarga de ordenar
 * el archivo mediante Replacement Selection
 *
 **/
class Sorter{
private:
     Sorter(){};
public:
	
	/**
	 * Funcion que se encarga de realizar
	 * el sort del archivo dado utilizando
	 * para ello, el metodo de Replacement
	 * Selection, y valiendose de un arbol
	 * rojo-negro para simplificar el manejo
	 * de los registros en memoria.
	 * 
	 * @param nombre El nombre del archivo a ordenar
	 * 		  nombreBaseSalida El nombre base de las 
	 * 			particiones de salida, a esto se le
	 * 			agregara un numero que indica el 
	 * 			numero de cada particion.
	 * 		  cantMax Es el tamaño de la memoria,
	 * 			en este caso es la cantidad de registros
	 * 			a agregar en el arbol.
	 **/
     static int Sort(const std::string &nombreArchivo,			\
		     const std::string &nombreBaseSalida,		\
		     unsigned cantMax);

};

#endif //__SORT_H_INCLUDED__
