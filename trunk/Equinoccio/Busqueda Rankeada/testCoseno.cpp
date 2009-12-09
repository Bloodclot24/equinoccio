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
	RedBlackTree<BusquedaRankeada::RegConsulta> arbol;
	BusquedaRankeada::RegConsulta *res;
	BusquedaRankeada::RegConsulta comp(0,0);
//
//	if (BusquedaRankeada::coseno(consulta,cat,arbol)) {
//
//		while (res = arbol.RemoverMayorIgual(comp)) {
//			std::cout<<"Doc "<<res->nro<<" 	peso: "<<1 - res->peso<<std::endl;
//			delete res;
//		}
//		return 0;
//	}
	std::cout<<"Error"<<std::endl;
	return 1;
}
