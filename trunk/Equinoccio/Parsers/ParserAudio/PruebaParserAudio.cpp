#include "ParserAudio.h"

int main(int argc, char** argv) {
	
	if(argc != 3) {
		std::cerr << "Indique ruta de un archivo de audio" << std::endl;
		return 1;
	}

	ParserAudio parser(15);

	//Archivo 1	
	std::cout << "Intentando Parsear: " << argv[1] << std::endl;
	if(parser.parsear(argv[1], 0))
		std::cout << "El archivo pudo ser parseado" << std::endl;
	else
		std::cerr << "No se pudo parsear el archivo" << std::endl;
	
	std::cout << std::endl;
	//Archivo 2
	std::cout << "------------------------------------------" << std::endl;
	std::cout << "Intentando Parsear: " << argv[2] << std::endl;
	if(parser.parsear(argv[2], 0))
		std::cout << "El archivo pudo ser parseado" << std::endl;
	else
		std::cerr << "No se pudo parsear el archivo" << std::endl;
	
	return 0;
}