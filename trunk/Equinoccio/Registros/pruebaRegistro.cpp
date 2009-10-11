#include "Registro.h"

#include <iostream>

int main(int argc, char** argv){
    if(argc<2){
	  std::cout << "La sintaxis es: " << argv[0] << " <archivo>\n";
	  return(-1);
    }
    std::ifstream archivo(argv[1], std::fstream::in);

    bool finalizado = false;
    do{
	  Registro *r = Registro::leer(archivo,0);
	  Registro r2("termino4", 2);
	  if(r->obtenerTermino().size() != 0){
	       r->unir(r2);
	       std::cout << "Termino: " << r->obtenerTermino() << "->";
	       const std::list<Registro::Punteros>& punteros = r->getPunteros();
	       std::list<Registro::Punteros>::const_iterator it;
	       for(it=punteros.begin(); it!=punteros.end();it++){
		    std::cout << "(" << (*it).documento << ";" << (*it).frecuencia << ")";
	       }
	       std::cout << std::endl;
	  }
	  else 
	       finalizado = true;
    }while(!finalizado);

    archivo.close();
}
