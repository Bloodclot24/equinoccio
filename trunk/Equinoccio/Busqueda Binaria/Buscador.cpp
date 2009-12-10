#include "Buscador.h"
#include "../FileManager/ConstPath.h"
#include "../FileManager/FileManager.h"
#include "../Notificador/Notificador.h"

RegistroIndice Buscador::buscar(const std::string& termino,const std::string& catalogo, uint32_t segmento)
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
	std::string indice =  FileManager::obtenerPathBase(segmento);
	indice += catalogo + ".idx";
	std::string lexico =  FileManager::obtenerPathBase(segmento);
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

RegistroNGrama Buscador::buscarNgrama(const std::string ngrama,const std::string& catalogo, uint32_t segmento)
{
	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	bool encontrado = false;
	bool error = false;
	std::string archNgrama =  FileManager::obtenerPathBase(segmento);
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

bool Buscador::buscarNroTermino(const std::string &termino, const std::string &catalogo, uint32_t& nro, uint32_t segmento) {

	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	std::fstream archIdx;
	std::fstream archLex;
	uint32_t pLexico;
	char cadena[50];
	bool error = false;
	bool encontrado = false;

	std::string indice =  FileManager::obtenerPathBase(segmento);
	indice += catalogo + ".idx";
	std::string lexico =  FileManager::obtenerPathBase(segmento);
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
		archIdx.read((char*)&(pLexico),sizeof(uint32_t));
		archLex.seekg(pLexico,std::fstream::beg);
		archLex.get((char*)cadena,50,'\0');

		if(termino.compare(cadena) == 0){
			encontrado = true;
			nro = medio;
		}else{
			if (izquierda==derecha && izquierda==medio) {
				encontrado = false;
				error = true;
			}else if(termino.compare(cadena) < 0) derecha = medio - 1;
			else
				izquierda = medio + 1;
		}
	}
	//si no lo encontre, o se produjo un error, devuelvo 0
	if((!encontrado) || (error)){
		encontrado = false;
	}

	return encontrado;
}

bool Buscador::buscarNroDirectorio(const std::string &path, uint32_t &nro, uint32_t &pun, uint32_t segm) {

	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	std::fstream dir_Idx;
	std::fstream dir_Lex;
	uint32_t pLexico;
	std::string ruta;
	bool error = false;
	bool encontrado = false;

	// Abro los archivos para lectura
	dir_Idx.open(FileManager::obtenerPathIdxDirs(segm).c_str(), std::fstream::in);
	dir_Lex.open(FileManager::obtenerPathLexDirs(segm).c_str(), std::fstream::in);

	//si se produjo un error al abrirlos cambio el flag
	if ((!dir_Lex.is_open()) || (!dir_Idx.is_open())) error = true;

	dir_Idx.seekg(0,std::fstream::end);
	derecha = dir_Idx.tellg()/sizeof(uint32_t);

	while ((!encontrado) && (izquierda <= derecha) && (!error)){
		//calculo la mitad
		medio = (derecha + izquierda) / 2;
		dir_Idx.seekg(medio*sizeof(uint32_t));
		dir_Idx.read((char*)&(pLexico),sizeof(uint32_t));
		dir_Lex.seekg(pLexico,std::fstream::beg);
		//archLex.get((char*)cadena,50,'\0');
		ruta.clear();
		std::getline(dir_Lex,ruta,'\0');

		if(ruta.compare(path) == 0){
			encontrado = true;
			nro = medio;
			//dir_Idx.seekg((medio+1)*sizeof(uint32_t));
			if (dir_Idx.good())
				dir_Idx.read((char*)&(pun),sizeof(uint32_t));
			else
				pun=0;
		}else{
			if (izquierda==derecha && izquierda==medio) {
				encontrado = false;
				error = true;
			}else if(path.compare(ruta) < 0) derecha = medio - 1;
			else
				izquierda = medio + 1;
		}
	}
	//si no lo encontre, o se produjo un error, devuelvo 0
	if((!encontrado) || (error)){
		encontrado = false;
	}
	return encontrado;
}

bool Buscador::buscarArchivodeDir(std::fstream &idx_arch, uint32_t nro, uint32_t &pun) {
	nro--;
	uint32_t medio = 0;
	uint32_t izquierda = 0;
	uint32_t derecha = 0;
	uint32_t nDir=0;
	bool error = false;
	bool encontrado = false;

	idx_arch.seekg(0,std::fstream::end);
	derecha = idx_arch.tellg()/sizeof(RegistroArchivo::size());

	while ((!encontrado) && (izquierda <= derecha) && (!error)){
		//calculo la mitad
		medio = (derecha + izquierda) / 2;
		idx_arch.seekg((medio*sizeof(RegistroArchivo::size()))+4);
		idx_arch.read((char*)&(nDir),sizeof(uint32_t));

		if(nro == nDir){
			encontrado = true;
			//encontre el valor menos uno, entonces busco hasta encontrar el dir siguiente
			do {
				medio++;
				idx_arch.seekg((medio*sizeof(RegistroArchivo::size()))+4);
				idx_arch.read((char*)&(nDir),sizeof(uint32_t));
			}while (nro == nDir && idx_arch.good());
			if (nDir==nro && !idx_arch.good()) //llegue al eof
				error = true;
			pun = medio;

		}else{
			if (izquierda>=medio) {
				do {
					medio++;
					idx_arch.seekg((medio*sizeof(RegistroArchivo::size()))+4);
					idx_arch.read((char*)&(nDir),sizeof(uint32_t));
				}while (nDir <= nro && idx_arch.good());
				if (nDir <= nro && !idx_arch.good())
					error = true;
				pun = medio;
			}else if(nro< nDir) derecha = medio - 1;
			else
				izquierda = medio + 1;
		}
	}
	//si no lo encontre, o se produjo un error, devuelvo 0
	if((!encontrado) || (error)){
		encontrado = false;
	}
	return encontrado;

}
