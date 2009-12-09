#include "BusquedaRankeada.h"

int main(int argc,char** argv){
	BusquedaRankeada armador;
	std::string catalogo(argv[1]);
	
	armador.armarMatrizCoseno(catalogo,0,0);
	

	return 0;	
}
