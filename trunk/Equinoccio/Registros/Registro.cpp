#include "Registro.h"

Registro::Registro(const std::string& termino, uint32_t documento){
     Registro::Punteros puntero;
     this->termino = termino;
     frecuencia = 1;
     puntero.documento = documento;
     puntero.frecuencia = 1;
     punteros.push_back(puntero);
}

Registro* Registro::leer(std::ifstream &archivo, int compresion){
     Registro* r= new Registro();;
     char c=-1;
     while(archivo.good() && (c = archivo.get()) != 0){
	  r->termino += c;
     }

     if(archivo.good())
	  archivo.read((char*)&(r->frecuencia), sizeof(r->frecuencia));
     else{
	  delete r;
	  return NULL;
     }

     uint32_t contador = r->frecuencia;
     Registro::Punteros p;
     while(archivo.good() && contador > 0){
	  if(!compresion){
	       archivo.read((char*)&(p.documento), sizeof(p.documento));
	       archivo.read((char*)&(p.frecuencia), sizeof(p.frecuencia));
	       r->punteros.push_back(p);
	       contador--;
	  }
	  else{
	       #warning "Falta implementar Gamma."
               std::string str;
               //uint32_t distancia= TDA_Codigo::getNGamma(str);
               //r->punteros.push_back(distancia);
	  }
     }

     return r;
}

int Registro::escribir(std::ofstream &archivo, int compresion){
     if(!archivo.good())
	  return 0;
     if(punteros.size() == 0)
	  return 0;

     archivo.write(termino.c_str(), termino.length()+1); //+1 para que escriba el \0
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
       #warning "Falta implementar Gamma."
       char caracter;
       bool doc= false;
       std::string str;
       char cadena;
       int j = 0;
       for(it=punteros.begin(); it != punteros.end(); it++){
         Registro::Punteros p;
	 p = *it;
	 if(doc) {
	   str= TDA_Codigos::getCGamma(p.documento);
	   doc= true;
	 } else {
	   str= TDA_Codigos::getCGamma(p.frecuencia);
	   doc= false;
	 }
	       	
	 for(unsigned int i = 0; i < str.length(); i++){
	   caracter= str[i];
	   int bit;
	   for(bit=1<<7;bit!=0 ;bit>>=1,j++){/*Shifteo*/
	     cadena+= (caracter>0?bit:0);
	     if(j == 7){
	       archivo.write((char*)&(cadena), sizeof(cadena));
	       j= 0;
	       cadena= 0;
	     }
	   }
	 }
       }
	  	
       if (cadena != 0){
         int bit;
         for(bit=j<<(7-j);bit!=0 ;bit>>=1,j++)/*Shifteo*/
           cadena += (caracter>0?bit:0);
           archivo.write((char*)&(cadena),sizeof(cadena));
	 }
       }

     return 1;
}

const std::string& Registro::obtenerTermino(){
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

     frecuencia=0;

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
	  frecuencia++;
     }
     
     while(it1!= punteros.end()){
	  final.push_back(*it1);
	  it1++;
	  frecuencia++;
     }
     while(it2!= registro.punteros.end()){
	  final.push_back(*it2);
	  it2++;
	  frecuencia++;
     }

     punteros = final;
     
     return 1;
}
