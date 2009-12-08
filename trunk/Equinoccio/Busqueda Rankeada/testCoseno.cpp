#include "BusquedaRankeada.h"

int main(int argc, char* argv[]) {

	std::string consulta="";
	for (int i=2; i<argc; i++) {
		consulta +=argv[i];
		if (i<(argc-1)) {
			consulta +=" ";
		}
	}
	std::string cat = argv[1];
	if (BusquedaRankeada::coseno(consulta,cat)) {
		return 0;
	}
	std::cout<<"Error"<<std::endl;
	return 1;
}
