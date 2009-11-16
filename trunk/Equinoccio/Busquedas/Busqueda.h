#ifndef BUSQUEDA_H_
#define BUSQUEDA_H_

#include <list>
#include <vector>
#include <string>
#include <stdint.h>

class Busqueda {

private:

	std::list<uint32_t> punteros_match;	//punteros resultantes de la consulta
	std::vector< std::list<uint32_t>* > punteros; //contiene las listas de punteros de todos los terminos buscados
	unsigned int size; //cantidad de listas de punteros


	void borrarListas();
	/* Busca la palabra en el catalogo indicado
	 * agrega la lista de punteros obtenidas en el vector de punteros
	 * devuelve false si NO se encontro en el indice
	 *
	 * */
	bool buscarEnIndice(std::string consulta, std::string catalogo);

	//dado un puntero devuelve el path del archivo
	std::string buscarPath(uint32_t, std::string catalogo);

public:
	Busqueda();
	virtual ~Busqueda();


	/* busca el string en el indice y devuelve una lista con los
	 * path de los documentos matcheados
	 * devuelve lista vacia en caso de que no haya match
	 * */
	std::list<std::string> buscar(std::string& consulta, std::string catalogo);


};

#endif
