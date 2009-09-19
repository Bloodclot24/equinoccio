#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <string.h>
#include <iostream>
#include "../Registros/Registro.h"
#include "../Util/Util.h"

/** 
 * Interfaz a implementar por cada uno de los parsers del programa.
 */

class Parser{

protected:

	uint32_t archivos;
	std::list<std::string> lista;
	
	/**
	 	* La funcion devuelve una cadena en minuscula, sin caracteres invalidos;
	  * Notar que los caracteres invalidos son convertidos en espacios, por lo
	  * que puede devolverse una cadena con mas de una palabra o una cadena 
	  * vacia.
	 	*/ 
		std::string aMinuscSinInvalidos(std::string informacion);
		
	/**
	 	* La funcion toma una cadena, filtra los espacios y guarda palabra a
	 	* palabra.
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
		
     /** El método intenta abrir el archivo con el nombre dado e
      * intena parsearlo. Si no puede, devuelve un string vacío. Si
      * puede, lo hace y vuelca los registros resultantes en el
      * archivo cuyo nombre devuelve en el string. El nombre del
      * archivo final se arma concatenando el nombre del archivo
      * original y "_dump".
      *
      * @param nombre El nombre del archivo a parsear @return Un
      * string vacío si no puede parsear el archivo o el nombre del
      * archivo con los registros resultantes.
      *
      * @param documento El numero de documento.
      * 
      * @see Registro
      */
     virtual std::string parsear(std::string nombre, uint32_t documento)=0;

		 /** 
		  * Devuelve la cantidad de archivos que genero parseando.
    	*/     
     uint32_t getCantArchivosParseados();		
};

#endif //PARSER_H_INCLUDED
