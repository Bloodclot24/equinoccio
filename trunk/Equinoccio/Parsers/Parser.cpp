#include "Parser.h"

/****************************************************************************/
void Parser::minusculaNoAcentuado(std::string& palabra) {

	//Suspenso	
}

/*--------------------------------------------------------------------------*/
//TODO: esto tiene problemas con los acentos....
void Parser::filtrarPalabra(std::string& palabra) {
	
	for(size_t i=0; i<palabra.size(); i++) {
		
		int ascii= (int) palabra[i];
	
		if(!(ascii >= 65 && ascii <= 90) && !(ascii >= 97 && ascii <= 122) &&
		   ascii != 164 && ascii != 165)
				palabra.replace(i, 1, " ");
	}	
}

/****************************************************************************/