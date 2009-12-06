#include "RegistroMatriz.h"
#include <iostream>


RegistroMatriz::RegistroMatriz(uint32_t x, uint32_t y, double peso){
	registro.x = x;
	registro.y = y;
	registro.peso = peso;
}

RegistroMatriz* RegistroMatriz::leer(std::ifstream &archivo, int compresion){
     RegistroMatriz* registroM = new RegistroMatriz();
     archivo.read((char*)&registroM->registro.x,sizeof(uint32_t));
     archivo.read((char*)&registroM->registro.y,sizeof(uint32_t));
     archivo.read((char*)&registroM->registro.peso,sizeof(double));
     if(archivo.good()) return registroM;
     else{
	 delete registroM;
	 return NULL;
     }


}

int RegistroMatriz::escribir(std::ofstream &archivo, int compresion){
    std::cout << "voy a escribir un registro en la matriz xD \n";
    std::cout << " x: " << registro.x << std::endl;
    std::cout << "y: " << registro.y << std::endl;
    std::cout << "peso: " << registro.peso << std::endl;
    archivo.write((char*)&registro.x,sizeof(uint32_t));
	archivo.write((char*)&registro.y,sizeof(uint32_t));
	archivo.write((char*)&registro.peso,sizeof(double));
	return 1;
}
