#include "Bitmap.h"
#include <string>
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv){

     if(argc < 2){
	  std::cout << "La sintaxis es: " << argv[0] << "<nombre archivo>\n";
	  return -1;
     }

     Bitmap mapa(argv[1]);

     std::cout << "Creando el mapa " << argv[1] << ".\n";

     uint32_t max =0;
     for(int i=0; i<10; i++){
	  uint32_t j= rand()%50;
	  std::cout << "Poniendo en 1 el bit " << j << "\n";
	  mapa.setBit(j,1);
	  if(j>max)
	       max=j;
     }

     for(uint32_t i=0;i<=max;i++){
	  std::cout << "Valor del bit " << i << ": "<< mapa.getBit(i) << "\n";
     }

     return 0;
}
