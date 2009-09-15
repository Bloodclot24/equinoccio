#include "TDA_Codigos.h"
int main(){/*Main de Prueba*/
	unsigned int nro=292929443; /*Para Probar Cambiar el Nro*/
	//TDA_Codigos codi(nro);
	std::string aux = TDA_Codigos::getCGamma(nro);
	std::cout << "El Codigo Gamma Codificado es:" << aux << std::endl;
	std::cout << "El Codigo Gamma Decodificado es:" << TDA_Codigos::getNGamma(aux) << std::endl;
}
