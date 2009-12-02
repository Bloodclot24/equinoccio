#include "Registro.h"
#include "../Util/Util.h"
#include <iostream>

int main(int argc, char** argv){
    if(argc<2){
	 // se espera un nombre de archivo
	  std::cout << "La sintaxis es: " << argv[0] << " <archivo> [compresion]\n";
	  return(-1);
    }
    int compresion = 0;
    if(argc> 2)
	 compresion = 1;

    // abro el archivo.
    std::ifstream archivo(argv[1], std::fstream::in);

    bool finalizado = false;
    do{
	 // leo un registro
	  Registro *r = Registro::leer(archivo,compresion);
	  if(r && r->obtenerTermino().size() != 0){
	       // muestro el termino
	       std::cout << "Termino: " << r->obtenerTermino() << "->";
	       const std::list<Registro::Punteros>& punteros = r->getPunteros();
	       std::list<Registro::Punteros>::const_iterator it;
	       // muestro todos los punteros que almacena
	       for(it=punteros.begin(); it!=punteros.end();it++){
		    std::cout << "(" << Util::intToString((*it).documento) << ";" << Util::intToString((*it).frecuencia) << ")";
	       }
	       std::cout << std::endl;
	  }
	  else 
	       finalizado = true;
    }while(!finalizado);
    
    archivo.close();
}
