#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include "../Registros/Registro.h"
#include "../Util/Util.h"
#include "../Arbol/RedBlackTree.h"

/** 
 * Interfaz a implementar por cada uno de los parsers del programa.
 */

class Parser{

protected:

	uint32_t archivos;
	uint32_t cantMaxReg;
	uint32_t cantReg;
	std::list<std::string> lista;
	RedBlackTree<std::string> arbol;
	
	/**
		* La funcion se encarga de cargar el arbol con los stop word.
		* 
    * @param nombreStopWord El archivo donde estan los stop word.
	 	*/ 
		void cargarStopWord(std::string nombreStopWord);
	
	/**
	 	* La funcion devuelve una cadena en minuscula, sin caracteres invalidos;
	  * Notar que los caracteres invalidos son convertidos en espacios, por lo
	  * que puede devolverse una cadena con mas de una palabra o una cadena 
	  * vacia.
	 	*/ 
		std::string aMinuscSinInvalidos(std::string informacion);
		
	/**
	 	* La funcion toma una cadena, filtra los espacios y guarda palabra a
	 	* palabra, en una lista auxiliar.
	 	*/ 
		void guardarPalabras(std::string palabras);

public:
     /** 
      * Crea un nuevo Parser, con un numero maximo de registros
      * por archivo.
      * 
      * @param cantMaxReg La cantidad maxima de registros por archivo.
      */
		 Parser(uint32_t cantMaxReg);
		
     /** 
      * El método intenta abrir el archivo con el nombre dado e
      * intenta parsearlo. Si puede, lo hace y vuelca los registros
      * resultantes en un archivo dump. Cada archivo dump, tiene
      * una cantidad maxima de registros que puede almacenar, el cual
      * es dado por parametro en el constructor. Si se supera esa
      * cant maxima se crea otro archivo dump. El formato de las
      * ruta de los archivos dump es: dump_tipoParser_#archivo
      *
      * @param nombre El nombre del archivo a parsear.
      * @param documento El numero de documento.
      * @return TRUE si pudo realizar el parseo, FALSE caso contrario.
      * 
      * @see Registro
      */
     virtual bool parsear(std::string nombre, uint32_t documento)=0;

		 /** 
		  * Devuelve la cantidad de archivos que genero parseando.
    	*/     
     uint32_t getCantArchivosParseados();
     
     /** 
		  * Devuelve la cantidad de registros del ultimo archivo.
    	*/     
     uint32_t getCantReg();			
};

#endif //PARSER_H_INCLUDED
