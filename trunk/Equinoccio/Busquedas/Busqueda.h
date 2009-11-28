#ifndef BUSQUEDA_H_
#define BUSQUEDA_H_

#include <list>
#include <vector>
#include <string>
#include <stdint.h>

class Busqueda {

private:

	typedef struct {
		std::string termino;
		uint32_t frec;
		uint32_t pun;
	}RegIndice;

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

	/*
	 * Dado un puntero devuelve el path del documento
	 *
	 * @param puntero puntero al indice de documentos
	 * @param catalogo catalogo al que pertenece el documento
	 *
	 * @return path del documento
	 * */
	std::string buscarPath(uint32_t puntero, std::string catalogo);

	/* realiza la consulta con ngramas
	 * @return false si no hay match
	 *  */
	bool consultaNgramas(std::string& consulta, std::string catalogo);


	/*
	 * realiza un and de las listas de registros de los bigramas
	 * comparando los terminos
	 *
	 * @param reg_bigrama 	contiene las listas de registros con los terminos para el and
	 * @param reg_and 		lista resultante con el AND de los registros
	 * */
	void andTerminos(std::vector<std::list<RegIndice*>*> &reg_bigrama, std::list<RegIndice*>& reg_and);


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
