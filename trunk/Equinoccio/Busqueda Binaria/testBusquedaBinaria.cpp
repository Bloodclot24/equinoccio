#include "Buscador.h"

int main(int argc, char* argv[]) {

	const std::string termino= argv[1];
	RegistroIndice regResultado;
	uint32_t segmentos= 3;

	std::cout << "Total segmentos: " << segmentos << std::endl;

	for(uint32_t i= 0; i<=segmentos-1; i++) {
		regResultado= Buscador::buscar(termino, "SND", i);
		std::cout << "-- Resultados Segmento " << i << " --" << std::endl;
		if(regResultado.frec != 0) {
			std::cout << "Frecuencia: " << regResultado.frec << std::endl;
			std::cout << "pLexico: " << regResultado.pLexico << std::endl;
			std::cout << "pDocs: " << regResultado.pDocs << std::endl;
		} else {
			std::cout << "No se encontro nada" << std::endl;
		}
	}

	return 0;
}
