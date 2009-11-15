#include "Busqueda.h"

//recibe el catalogo primero y despues la consulta

int main (int argc, char** argv) {

	std::string consulta="";
	for (int i=2; i<argc; i++) {
		consulta +=argv[i];
	}

	Busqueda buscador;
	buscador.buscar(consulta, argv[1]);

	return 0;
}
