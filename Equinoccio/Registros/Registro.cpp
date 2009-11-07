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
     Registro* r= new Registro();
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
     

     if(!compresion){
	  while(archivo.good() && contador > 0){
	       archivo.read((char*)&(p.documento), sizeof(p.documento));
	       archivo.read((char*)&(p.frecuencia), sizeof(p.frecuencia));
	       r->punteros.push_back(p);
	       contador--;
	  }
	  
     }
     else{

	  char byte=0;
	  unsigned bit=1<<7;
	  uint32_t docAnterior = 0;
	  unsigned bits=0;
	  unsigned indice=0;
	  uint32_t valores[2];
	  std::string aux;

	  while(archivo.good() && contador > 0){
	       if(bits==0){
		    archivo.read(&byte, 1);
		    bits=8;
		    bit=1<<7;
	       }

	       for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
		    aux += byte&bit>0?'1':'0';
		    
		    if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1)
			 indice++;
	       }
	       if(indice==2){
		    docAnterior = p.documento = valores[0]+docAnterior;
		    p.frecuencia=valores[1];
		    r->punteros.push_back(p);
		    indice=0;
		    contador--;
	       }
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

	  const char* ptr;
	  std::string str1, str2;
	  char byte=0;
	  unsigned bit=1<<7;
	  uint32_t docAnterior = 0;
	  unsigned bits=0;

	  for(it=punteros.begin(); it != punteros.end(); it++){
	       Registro::Punteros p;
	       p = *it;

	       str1= TDA_Codigos::getCGamma(p.documento-docAnterior+1);
	       docAnterior=p.documento;

	       str2=TDA_Codigos::getCGamma(p.frecuencia);

	       str1+=str2;

	       ptr  = str1.c_str();
	       bits = str1.length();
	       while(bits > 0){
		    for(;bit!=0 && bits > 0; bit >>= 1, bits--, ptr++){
			 byte |= *ptr!='0'?bit:0;
		    }
		    if(bit==0){
			 /* escribir el byte a disco */
			 archivo.write(&byte,1);
			 bit=1<<7;
			 byte=0;
		    }
	       }
	  }
	  if(bit != 1<<7){
	       archivo.write(&byte,1);
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
