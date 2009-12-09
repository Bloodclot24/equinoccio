#include "TDA_Codigos.h"
#include <string>
int main(int argc, char **argv){/*Main de Prueba*/
     //unsigned int nro=292929443; /*Para Probar Cambiar el Nro*/
	//TDA_Codigos codi(nro);
     std::string aux = TDA_Codigos::getCGamma(atoi(argv[1]));
	std::cout << "El Codigo Gamma Codificado es:" << aux << std::endl;
	std::cout << "El Codigo Gamma Decodificado es:" << TDA_Codigos::getNGamma(aux) << std::endl;
}
