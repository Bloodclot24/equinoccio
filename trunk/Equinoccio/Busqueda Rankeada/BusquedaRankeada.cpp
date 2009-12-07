#include "BusquedaRankeada.h"
#include <math.h>

void BusquedaRankeada::armarMatrizCoseno(std::string& catalogo){
     std::string path;
     // TODO: en cual segmento??
	path = FileManager::obtenerPathBase(0);
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
     // TODO: en cual segmento??
	path = FileManager::obtenerPathBase(0);
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
	path = FileManager::obtenerPathBase(0);
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
	uint32_t cantTerminos = 340350;
	uint32_t cantidadDocumentos = 28209;
	//****************************************************************

	uint32_t pTermino = 0;
	uint32_t frecGlobal = 0;
	uint32_t pDocs = 0;
	uint32_t frecLocal = 0;
	uint32_t documento = 0;
	double peso = 0.0;
	double puntajeTermino = 0.0;
	double norma = 0.0;

	std::ofstream puntajes;
	// TODO: en cual segmento??
	std::string ruta = FileManager::obtenerPathBase(0);
	ruta += catalogo;
	ruta += ".frec";
	puntajes.open(ruta.c_str(),std::ios::out | std::ios::trunc);

	if(!puntajes.good()){
		std::cerr << "Error al crear  el archivo de puntajes de terminos para la matriz de cosenos!!" << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}

	for(uint32_t i = 0; i < cantTerminos; i++){
		indice.read((char*)&pTermino, sizeof(uint32_t));
		indice.read((char*)&frecGlobal, sizeof(uint32_t));
		indice.read((char*)&pDocs, sizeof(uint32_t));
		std::list<Registro::Punteros>* punt = new std::list<Registro::Punteros>;
		Registro::obtenerPunterosEnLista(punteros,pDocs,frecGlobal,punt);
		std::list<Registro::Punteros>::iterator it;

		puntajeTermino = log10((double) cantidadDocumentos/frecGlobal);
	    puntajes.write((char*)&puntajeTermino,sizeof(double));

		for(it = punt->begin(); it != punt->end(); it++){
		    std::cout << "Entre al bucle de punteros:P \n";
		    std::cout << "frecuencia local: " << (*it).frecuencia << std::endl;
		    std::cout << "cantidadDocumentos/frecGlobal: " << (double) cantidadDocumentos/frecGlobal << std::endl;
		    peso = (double) (*it).frecuencia * puntajeTermino;//log10((double) cantidadDocumentos/frecGlobal);
		    RegistroMatriz registro(i,(*it).documento,peso);
		    std::cout << "Voy a escribir un registro en la matriz! " << std::endl;
		    std::cout << "Peso: " << peso<< std::endl;
		    std::cout << "x: " << i << std::endl;
		    std::cout << "y: " << (*it).documento << std::endl;
		    registro.escribir(matriz,0);
		}
		std::cout << "Bucle Afuera:P \n " ;
		delete punt;
	}


    matriz.close();
    indice.close();
    punteros.close();
    puntajes.close();

	std::cout << "VA A ENTRARRRRRR AL SORRTTTTTTTTTTTTTTTTT PARAAAAAAAAAAAAAALOOOOOOOOOOOOOOOOOOO!!!!!!!! \n";
	uint32_t cantParticiones = 0;
	//Transpongo la matriz.
	std::string nombreBase;
     // TODO: en cual segmento??
	nombreBase = FileManager::obtenerPathBase(0);
	nombreBase += "matrizSort";
	std::cout << "SORT!\n ";
	cantParticiones = Sorter<RegistroMatriz>::Sort(path,nombreBase,0,1000);
	std::cout << "Salio del SORT! \n";
	Parsers parsers;
	std::string nombreSalida;
     // TODO: en cual segmento??
	nombreSalida = FileManager::obtenerPathBase(0);
	nombreSalida += catalogo;
	nombreSalida += ".mat";
	parsers.merge<RegistroMatriz>(nombreBase,0,cantParticiones,nombreSalida);

	//guardo la matriz plegada.
	std::ifstream matrizTranspuesta;
	matrizTranspuesta.open(nombreSalida.c_str(), std::ios::in);
	if(!matrizTranspuesta.good()){
		std::cerr << "Error al abrir el archivo de la matriz de cosenos!!" << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}
	uint32_t x = 0;
	uint32_t y = 0;
	double valor = 0.0;
	uint32_t index = 0;
	path.clear();
	path = FileManager::obtenerPathBase(0);
	path += catalogo;
	path += ".mc";
	std::string path1 = path + "1";
	std::ofstream matCoseno1(path1.c_str(),std::ios::out | std::ios::trunc);
	std::string path2 = path + "2";
	std::ofstream matCoseno2(path2.c_str(),std::ios::out | std::ios::trunc);
	std::string path3 = path + "3";
	std::ofstream matCoseno3(path3.c_str(),std::ios::out | std::ios::trunc);
	if(!matCoseno1.good() || !matCoseno2.good() || !matCoseno3.good()){
		std::cerr << "Error al crear el archivo de la matriz de cosenos!!" << std::endl;
		#warning "ver como se va a manejar este error";
		return;
	}

	//Tengo que calcular la norma por cada documento, es decir
	//sumo los pesos de la fila de la matriz, y los voy guardando
	//para despues dividir por ella el peso de cada termino de la fila
	//ver si conviene en memoria o en disco.
	uint32_t colAnt = 0;
	uint32_t columna = 0;
	bool primero = true;
	bool cambieFila = false;
	uint32_t j = 0;

	std::vector<double> normas;

	while(matrizTranspuesta.good()){
		matrizTranspuesta.read((char*)&columna, sizeof(uint32_t));
		matrizTranspuesta.read((char*)&x, sizeof(uint32_t));
		matrizTranspuesta.read((char*)&valor, sizeof(double));

		if((colAnt != columna) && (!primero)){
			normas[j] = norma;
			norma = 0;
			primero = false;
			j++;
		}
		colAnt = columna;

		if(matrizTranspuesta.good()){
			norma += pow(valor,2);
		}
	}

	primero = true;
	cambieFila = false;
	uint32_t xAnt = 0;

	while(matrizTranspuesta.good()){
		matrizTranspuesta.read((char*)&y, sizeof(uint32_t));
		matrizTranspuesta.read((char*)&x, sizeof(uint32_t));
		matrizTranspuesta.read((char*)&valor, sizeof(double));

		if(xAnt != x) cambieFila = true;
		xAnt = x;

		if(valor && matrizTranspuesta.good()){
			matCoseno1.write((char*)&valor,sizeof(double));
			matCoseno2.write((char*)&y,sizeof(uint32_t));
			if(cambieFila || primero){
				matCoseno3.write((char*)&index,sizeof(uint32_t));
				primero = false;
				cambieFila = false;
			}
			index++;
		}

	}

	matrizTranspuesta.close();
	remove(nombreSalida.c_str());
	matCoseno1.close();
	matCoseno2.close();
	matCoseno3.close();

}
