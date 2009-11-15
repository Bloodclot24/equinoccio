#ifndef BUSCADOR_H_
#define BUSCADOR_H_

#include <iostream>
#include <fstream>

/* El tipo de archivo que devuelve como resultado de la busqueda.
 **/
typedef struct{
	uint32_t pLexico;
	uint32_t frec;
	uint32_t pDocs;
	
}RegistroIndice;

/* El Buscador es el encargado de realizar una busqueda binaria.
 **/

class Buscador
{
	public:
		/* Busqueda binaria de un termino sobre un catalogo determinado.
		 * 
		 * @param termino El termino a buscar.
		 * @param catalogo El tipo de catalogo.
		 **/
		static RegistroIndice buscar(const std::string termino, const std::string catalogo);	
};

#endif /*BUSCADOR_H_*/
