#include "Buscador.h"

RegistroIndice Buscador::buscar(const std::string termino,const std::string catalogo)
{
	int medio = 0;
	int izquierda = 0;
	int derecha = 0;
	bool encontrado = false;
	std::fstream archIdx;
	std::fstream archLex;
	RegistroIndice regInd;
	char cadena[50];
	bool error = false;
	const std::string indice = catalogo + ".idx";
	const std::string lexico = catalogo + ".lex";
	
	// Abro los archivos para lectura
	archIdx.open(indice.c_str(), std::fstream::in);
	archLex.open(lexico.c_str(), std::fstream::in);
	
	//si se produjo un error al abrirlos cambio el flag
	if ((!archIdx.is_open()) || (!archLex.is_open())) error = true;
	
	archIdx.seekg(std::fstream::end);
	derecha = archIdx.tellg();
	
	while ((!encontrado) && (izquierda < derecha) && (!error)){
		//calculo la mitad
		medio = (derecha - izquierda) / 2;
		archIdx.seekg(medio);
		archIdx.read((char*)regInd.pLexico,sizeof(uint32_t));
		archLex.seekg(regInd.pLexico);
		archLex.get(cadena,'\0');
		
		if(termino.compare(cadena) == 0){
			encontrado = true;
			archIdx.read((char*)regInd.frec,sizeof(uint32_t));
			archIdx.read((char*)regInd.pDocs,sizeof(uint32_t));
		
		}else{
			
			if(termino.compare(cadena) < 0) izquierda = medio;
			else 
				derecha = medio;
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

