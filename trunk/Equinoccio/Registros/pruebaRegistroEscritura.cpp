#include "Registro.h"

#include <iostream>

int main(int argc, char** argv){
     if(argc<2){
	  std::cout << "La sintaxis es: " << argv[0] << " <archivo>\n";
	  return(-1);
     }
     std::ofstream archivo(argv[1], std::fstream::out);

     Registro r1("palabra1", 1);
     r1.escribir(archivo, 0);
     Registro r2("palabra2", 2);
     r2.escribir(archivo, 0);
     Registro r3("palabra3", 3);
     r3.escribir(archivo, 0);
     Registro r4("palabra4", 4);
     r4.escribir(archivo, 0);

     archivo.close();
}
