#include "Buscador.h"

int main(int argc, char* argv[]) {

	const std::string termino= argv[2];
	uint32_t nro = 0;
	if (! Buscador::buscarNroTermino(termino, argv[1], nro)) {
		std::cout<<"no se encontro el termino en el indice"<<std::endl;
	}
	return 0;
}


