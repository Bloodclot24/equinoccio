#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>

/** 
 * Interfaz a implementar por cada uno de los parsers del programa.
 * 
 */
class Parser{
public:
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
      * @see Registro
      */
     std::string parsear(std::string nombre)=0;	

};

#endif //PARSER_H_INCLUDED
