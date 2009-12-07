#include "Buscador.h"
#include "../FileManager/ConstPath.h"
#include "../FileManager/FileManager.h"

RegistroIndice Buscador::buscar(const std::string& termino,const std::string& catalogo)
{
	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	bool encontrado = false;
	std::fstream archIdx;
	std::fstream archLex;
	RegistroIndice regInd;
	char cadena[50];
	bool error = false;
	// TODO: deberia buscar en todos los segmentos
	std::string indice =  FileManager::obtenerPathBase(0);
	indice += catalogo + ".idx";
	std::string lexico =  FileManager::obtenerPathBase(0);
	lexico += catalogo + ".lex";
	
	// Abro los archivos para lectura
	archIdx.open(indice.c_str(), std::fstream::in);
	archLex.open(lexico.c_str(), std::fstream::in);
	
	//si se produjo un error al abrirlos cambio el flag
	if ((!archIdx.is_open()) || (!archLex.is_open())) error = true;
	
	archIdx.seekg(0,std::fstream::end);
	derecha = archIdx.tellg()/sizeof(RegistroIndice);
	
	while ((!encontrado) && (izquierda <= derecha) && (!error)){
		//calculo la mitad
		medio = (derecha + izquierda) / 2;
		archIdx.seekg(medio*sizeof(RegistroIndice));
		archIdx.read((char*)&(regInd.pLexico),sizeof(uint32_t));
		archLex.seekg(regInd.pLexico,std::fstream::beg);
		archLex.get((char*)cadena,50,'\0');
		
		if(termino.compare(cadena) == 0){
			encontrado = true;
			archIdx.read((char*)&(regInd.frec),sizeof(uint32_t));
			archIdx.read((char*)&(regInd.pDocs),sizeof(uint32_t));
			std::cout << "La frecuencia es: " << regInd.frec << "\n";
		
		}else{
			if (izquierda==derecha && izquierda==medio) {
				encontrado = false;
				error = true;
			}else if(termino.compare(cadena) < 0) derecha = medio - 1;
			else
				izquierda = medio + 1;
		}		
	} 
	//si no lo encontre, o se produjo un error, devuelvo la estructura cargada de 0's
	if((!encontrado) || (error)){
 		regInd.pLexico = 0;
		regInd.frec = 0;
		regInd.pDocs = 0;
	}
	
	return regInd;
}

RegistroNGrama Buscador::buscarNgrama(const std::string ngrama,const std::string& catalogo)
{
	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	bool encontrado = false;
	bool error = false;
	// TODO: debería buscar en todos los segmentos
	std::string archNgrama =  FileManager::obtenerPathBase(0);
	archNgrama += catalogo + EXT_NG_IDX;
	std::fstream archivo;
	RegistroNGrama regNgrama;
	//char bigrama[2];
	archivo.open(archNgrama.c_str(),std::fstream::in);
	if(!archivo.is_open()) error = true;
	
	archivo.seekg(0,std::fstream::end);
	derecha = archivo.tellg()/regNgrama.sizeofNG();

	while (!encontrado && izquierda <= derecha && !error){
		medio = (izquierda + derecha) / 2;
		archivo.seekg(medio*RegistroNGrama::sizeofNG());
		archivo.read((char*)&(regNgrama.ngrama),sizeof(char)*2);
		
		if(ngrama.compare(0,2,regNgrama.ngrama,2) == 0){
			encontrado = true;
			archivo.read((char*)&(regNgrama.frec),sizeof(uint32_t));
			archivo.read((char*)&(regNgrama.pDocs),sizeof(uint32_t));
		
		}else{
			if (izquierda==derecha && izquierda==medio) {
				encontrado = false;
				error = true;
			}else if(ngrama.compare(regNgrama.ngrama) < 0) derecha = medio -1;
			else
				izquierda = medio +1;
		}		
	} 
	//si no lo encontre, o se produjo un error, devuelvo la estructura cargada de 0's
	if((!encontrado) || (error)){
		regNgrama.frec = 0;
		regNgrama.pDocs = 0;
	}
	
	return regNgrama;

}

