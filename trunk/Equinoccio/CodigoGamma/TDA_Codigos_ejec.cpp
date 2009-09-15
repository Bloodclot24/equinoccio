#include "TDA_Codigos.h"
int main(){
	int nro=7;
	TDA_Codigos* codi= new TDA_Codigos(nro);
	cout << "El nro es:" << nro; 
	cout << "\nEl Codigo Unario es:"<<codi->TDA_Codigos::getCUnario(nro);
	cout << "\nEl Codigo Gamma es:"<<codi->TDA_Codigos::getCGamma();
	delete (&codi);
}
