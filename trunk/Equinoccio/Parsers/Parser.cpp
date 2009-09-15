#include "Parser.h"

/****************************************************************************/
std::string Parser::aMinuscSinInvalidos(std::string informacion) {
	
	//TODO: /*PRUEBA*/
	std::cout << "Palabra Cruda: " << informacion << std::endl;
	
	std::string invalidos("!#$%&'()*+,-.0123456789:;<=>?@[]^_`{|}~");
	std::string palabraFiltrada;
	for(size_t c=0; c<informacion.size(); c++) {
		
		size_t pos= invalidos.find(informacion[c], 0);
		
		if(pos == std::string::npos)
			palabraFiltrada+= tolower(informacion[c]);
		else
			palabraFiltrada+= " "; 
	}

	//TODO: /*PRUEBA*/
	std::cout << "Palabra Filtrada: " << palabraFiltrada << std::endl;
	
	return palabraFiltrada;
}

/*--------------------------------------------------------------------------*/
void Parser::guardarPalabras(std::string palabras) {
	
	std::string strAparsear(palabras); 
	size_t found, pos= 0;
	
	do {
		found= strAparsear.find(" ", pos);
		std::string palabraAgregar(strAparsear, pos, found-pos);
		if(palabraAgregar.compare("") != 0)
			lista.push_back(palabraAgregar);	
		pos= found+1;
	} while(found != std::string::npos);
	
}

/****************************************************************************/