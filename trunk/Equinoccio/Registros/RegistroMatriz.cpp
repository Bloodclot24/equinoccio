#include "RegistroMatriz.h"


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
	return registroM;

}

int RegistroMatriz::escribir(std::ofstream &archivo, int compresion){
	archivo.write((char*)&registro.x,sizeof(uint32_t));
	archivo.write((char*)&registro.y,sizeof(uint32_t));
	archivo.write((char*)&registro.peso,sizeof(double));
	return 1;
}
