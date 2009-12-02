#include "Registro.h"

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

     std::ofstream archivo(argv[1], std::fstream::out);

     Registro r1("palabra1", 0);
     r1.escribir(archivo, compresion);
     Registro r2("palabra2", 2);
     r2.escribir(archivo, compresion);
     Registro r3("palabra3", 5);
     r3.escribir(archivo, compresion);
     Registro r4("palabra4", 7);
     r4.escribir(archivo, compresion);

     Registro r5("palabra5", 9);
     Registro r6("palabra5", 10);
     Registro r7("palabra5", 29);
     Registro r8("palabra5", 99);
     r5.unir(r6);
     r5.unir(r7);
     r5.unir(r8);
     r5.escribir(archivo, compresion);
	  

     

     archivo.close();
}
