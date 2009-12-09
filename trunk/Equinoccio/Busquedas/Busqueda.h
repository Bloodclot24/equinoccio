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

	/* Busca la palabra en el catalogo indicado
	 * agrega la lista de punteros obtenidas en el vector de punteros
	 * devuelve false si NO se encontro en el indice
	 *
	 * */
	bool buscarEnIndice(std::string consulta, std::string catalogo, uint32_t segmento);

	/*
	 * Dado un puntero devuelve el path del documento
	 *
	 * @param puntero puntero al indice de documentos
	 * @param catalogo catalogo al que pertenece el documento
	 * "param segmento segmento del archivo
	 *
	 * @return path del documento
	 * */
	std::string buscarPath(uint32_t puntero, std::string catalogo, uint32_t segmento);

	/* realiza la consulta con ngramas
	 * @return false si no hay match
	 *  */
	bool consultaNgramas(std::string& consulta, std::string catalogo);

	/*
	 * realiza un and de los punteros en las listas
	 *
	 * @param punteros	 	contiene las listas de punteros a intersectar
	 * @param punteros_and	lista resultante con el AND de los punteros
	 * */
	static void andPunteros(std::vector< std::list<uint32_t>* > &punteros, std::list<uint32_t> &punteros_and);

	static void andPunteros2(std::vector< std::list<uint32_t>* > &punteros, std::list<uint32_t> &punteros_and);

	/*
	 * Guarda en la lista filtrada los elementos de la lista original que contengan a los terminos. Para ngramas.
	 *
	 * @param consulta 	palabra a matchear
	 * @param lista		lista de registros
	 * @param filtrada	lista de registros que contienen en su termino a la palabra a matchear
	 * */
	void filtrarFalsosPositivos(std::list<std::string>& consulta, std::list<RegIndice*>& lista, std::list<RegIndice*>& filtrada);

	/*
	 * junta todos los punteros de las palabras matcheadas por ngramas, eliminando repeticiones.
	 * se asume que las listas de punteros tienen elementos
	 * @return 	puntero a una lista con los resultados. NULL en caso de error o de resultado vacio
	 * */

	std::list<uint32_t>* unionPunteros(std::vector< std::list<uint32_t>* > &punteros);
	std::list<uint32_t>* unionPunteros2(std::vector< std::list<uint32_t>* > &punteros);

public:
	Busqueda();
	~Busqueda();


	/* busca el string en el indice y devuelve una lista con los
	 * path de los documentos matcheados
	 * devuelve NULL en caso de que no haya match
	 * */
	std::list<std::string>* buscar(std::string& consulta, std::string catalogo);


};

#endif
