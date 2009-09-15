#include "Parser.h"

/****************************************************************************/
void sacarAcentuacion(std::string& palabra) {

	//Suspenso	
}

/*--------------------------------------------------------------------------*/
void Parser::filtrarPalabra(std::string& palabra) {
	
	for(size_t i=0; i<palabra.size(); i++) {
		
		unsigned int ascii= (int) palabra[i];
	
		if(!((ascii >= 65 && ascii <= 90) || (ascii >= 97 && ascii <= 122)) ||
		   ascii == 164 || ascii == 165)
			palabra.replace(i, 1, "");
	}	
}

/****************************************************************************/