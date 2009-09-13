#include "Registro.h"

Registro::Registro(std::string termino, uint32_t documento){
     Registro::Punteros puntero;
     this->termino = termino;
     frecuencia = 1;
     puntero.documento = documento;
     puntero.frecuencia = 1;
     punteros.push_back(puntero);
}

Registro Registro::leer(std::ifstream archivo, int compresion){
     Registro r;
     char c=-1;
     while(archivo.good() && c != 0){
	  c = archivo.get();
	  r.termino += c;
     }

     if(archivo.good())
	  archivo.read((char*)&(r.frecuencia), sizeof(r.frecuencia));
     else
	  return r;

     uint32_t contador = r.frecuencia;
     Registro::Punteros p;
     while(archivo.good() && contador > 0){
	  if(!compresion){
	       archivo.read((char*)&(p.documento), sizeof(p.documento));
	       archivo.read((char*)&(p.frecuencia), sizeof(p.frecuencia));
	       r.punteros.push_back(p);
	       contador--;
	  }
	  else{

	  }
     }

     return r;
}

int Registro::escribir(std::ofstream archivo, int compresion){
     if(!archivo.good())
	  return 0;
     if(punteros.size() == 0)
	  return 0;

     archivo.write(termino.c_str(), termino.length());
     archivo.write((char*)&frecuencia, sizeof(frecuencia));

     std::list<Registro::Punteros>::iterator it;
     if(compresion==0){
	  for(it=punteros.begin(); it != punteros.end(); it++){
	       Registro::Punteros p;
	       p = *it;
	       archivo.write((char*)&(p.documento), sizeof(p.documento));
	       archivo.write((char*)&(p.frecuencia), sizeof(p.frecuencia));
	  }
     }
     else{

     }

     return 1;
}

std::string Registro::obtenerTermino(){
     return termino;
}

int Registro::unir(const Registro& registro){
     std::list<Registro::Punteros> final;
     std::list<Registro::Punteros>::const_iterator it1, it2;

     if(termino != registro.termino)
	  return 0;

     if(punteros.size() == 0)
	  return 0;

     if(registro.punteros.size() == 0)
	  return 0;
     
     it1 = punteros.begin();
     it2 = registro.punteros.begin();
     
     while(it1!= punteros.end() && it2 != registro.punteros.end()){
	  if((*it1).documento < (*it2).documento){
	       final.push_back(*it1);
	       it1++;
	  }
	  else if((*it1).documento > (*it2).documento){
	       final.push_back(*it2);
	       it2++;
	  }
	  else{
	       Registro::Punteros p;
	       p.documento = (*it1).documento;
	       p.frecuencia = (*it1).frecuencia + (*it2).frecuencia ;
	       final.push_back(p);
	       it1++;
	       it2++;
	  }
     }
     
     while(it1!= punteros.end()){
	  final.push_back(*it1);
	  it1++;
     }
     while(it2!= registro.punteros.end()){
	  final.push_back(*it2);
	  it2++;
     }

     punteros = final;
     
     return 1;
}
