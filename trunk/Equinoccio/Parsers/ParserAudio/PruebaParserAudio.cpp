#include "ParserAudio.h"

int main(int argc, char** argv) {
	
	ParserAudio parser(40);
	
	if(argc != 2) {
		std::cerr << "Indique ruta de un archivo de audio" << std::endl;
		return 1;
	}
	
	std::string nombre_dump= parser.parsear(argv[1], 0);
	
	if(nombre_dump.compare("") != 0) {
		std::ifstream dump(nombre_dump.c_str(), std::fstream::in);
		
		//TODO: el archivo se crea bien, esto esta mal :p
//	  while(!dump.eof()) {
//			Registro reg= Registro::leer(dump,0);
//		  
//		  if(reg.obtenerTermino().size() != 0){
//	
//		    std::cout << "Termino: " << reg.obtenerTermino() << std::endl;
//		    std::cout << "Documento: " << 0 << std::endl;
//				std::cout << std::endl;	       
//			}
//	  }
	} else
		std::cerr << "No se pudo parsear el archivo" << std::endl;
	
	return 0;
}