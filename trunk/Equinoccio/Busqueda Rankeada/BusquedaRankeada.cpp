#include "BusquedaRankeada.h"
#include <math.h>

void BusquedaRankeada::armarMatrizCoseno(std::string& catalogo){
	std::string path;
	FileManager::setSegmento(999);
	path = FileManager::obtenerPathBase();
	path += catalogo;
	path += ".idx";
	std::fstream indice;
	indice.open(path.c_str(),std::ios::in);
	if(!indice.good()){
		std::cerr << "Error al abrir el indice para la matriz de cosenos!!" << std::endl;
		std::cout << "Nombre: " << path << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}
	path.clear();
	path = FileManager::obtenerPathBase();
	path += catalogo;
	path += ".pun";
	std::ifstream punteros;
	punteros.open(path.c_str(), std::ios::in);
	if(!punteros.good()){
		std::cerr << "Error al abrir el archivo de punteros para la matriz de cosenos!!" << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}

	std::ofstream matriz;
	path.clear();
	path = FileManager::obtenerPathBase();
	path += catalogo;
	path += ".matrix";

	matriz.open(path.c_str(),std::ios::out | std::ios::trunc);
	if(!matriz.good()){
		std::cerr << "Error crear  el archivo para la matriz de cosenos!!" << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}
	//TODO voy a tener la cantidad de terminos y de documentos en el indice.
	//Usar los metodos, obtenerCantidadTerminos y obtenerCantidadDocumentos
	//de Parsers.h
	uint32_t cantTerminos = 106815;
	uint32_t cantidadDocumentos = 6272;
	//****************************************************************

	uint32_t pTermino = 0;
	uint32_t frecGlobal = 0;
	uint32_t pDocs = 0;
	uint32_t frecLocal = 0;
	uint32_t documento = 0;
	double peso = 0;
	int j = 0;


	for(uint32_t i = 0; i < cantTerminos; i++){
		indice.read((char*)&pTermino, sizeof(uint32_t));
		indice.read((char*)&frecGlobal, sizeof(uint32_t));
		indice.read((char*)&pDocs, sizeof(uint32_t));
		std::list<Registro::Punteros>* punt = new std::list<Registro::Punteros>;
		Registro::obtenerPunterosEnLista(punteros,pDocs,frecGlobal,punt);
		std::list<Registro::Punteros>::iterator it;
		for(it = punt->begin(); it != punt->end(); it++){
		    std::cout << "Entre al bucle de punteros:P \n";
		    std::cout << "frecuencia local: " << (*it).frecuencia << std::endl;
		    std::cout << "cantidadDocumentos/frecGlobal: " << (double) cantidadDocumentos/frecGlobal << std::endl;
		    peso = (double) (*it).frecuencia * log10((double) cantidadDocumentos/frecGlobal);
		    RegistroMatriz registro(i,(*it).documento,peso);
		    std::cout << "Voy a escribir un registro en la matriz! " << std::endl;
		    std::cout << "Peso: " << peso<< std::endl;
		    std::cout << "x: " << i << std::endl;
		    std::cout << "y: " << (*it).documento << std::endl;
		    registro.escribir(matriz,0);
		    //vNoNulos.push_back(peso);
//			vColumna.push_back((*it).documento);
//			vIndice.push_back(j);
			j++;
		}
		std::cout << "Bucle Afuera:P \n " ;
	}


        matriz.close();
	std::cout << "VA A ENTRARRRRRR AL SORRTTTTTTTTTTTTTTTTT PARAAAAAAAAAAAAAALOOOOOOOOOOOOOOOOOOO!!!!!!!! \n";
	uint32_t cantParticiones = 0;
	//Transpongo la matriz.
	std::string nombreBase;
	nombreBase = FileManager::obtenerPathBase();
	nombreBase += "matrizSort";
	std::cout << "SORT!\n ";
	cantParticiones = Sorter<RegistroMatriz>::Sort(path,nombreBase,0,1000);
	std::cout << "Salio del SORT! \n";
	Parsers parsers;
	std::string nombreSalida;
	nombreSalida = FileManager::obtenerPathBase();
	nombreSalida += catalogo;
	nombreSalida += ".mat";
	parsers.merge<RegistroMatriz>(nombreBase,0,cantParticiones,nombreSalida);
}
