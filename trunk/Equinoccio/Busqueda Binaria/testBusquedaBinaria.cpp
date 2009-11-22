#include "Buscador.h"

//CATALOGO MUSICA
int main(int argc, char* argv[]) {

	RegistroIndice regResultado;
	const std::string termino= argv[1];

	regResultado= Buscador::buscar(termino, "SRC");

	std::cout << "-- Resultado --" << std::endl;
	if(regResultado.frec != 0) {
		std::cout << "Frecuencia: " << regResultado.frec << std::endl;
		std::cout << "pLexico: " << regResultado.pLexico << std::endl;
		std::cout << "pDocs: " << regResultado.pDocs << std::endl;
	} else {
		std::cout << "No se encontro nada" << std::endl;
	}

	return 0;
}
