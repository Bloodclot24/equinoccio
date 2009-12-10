#include "Registro.h"
#include <sstream>
#include "../Util/Util.h"

Registro::Registro(const std::string& termino, uint32_t documento){
     Registro::Punteros puntero; // punteros vacios
     this->termino = termino;
     frecuencia = 1;
     puntero.documento = documento; // asigno numero de documento
     puntero.frecuencia = 1;	    // asigno frecuencia inicial
     punteros.push_back(puntero);   // agrego el puntero a la lista
}

Registro::Registro(const std::string& termino){
     this->termino = termino;
     frecuencia=0;
}

std::list<Registro::Punteros>& Registro::getListaPunteros(){
     return punteros;
}

Registro* Registro::leer(std::ifstream &archivo, int compresion){
     Registro* r= new Registro(); // creo un registro
     char c=-1;
     while(archivo.good() && (c = archivo.get()) != 0){
	  r->termino += c;
     } // leo hasta el \0 y voy armando el termino

     if(archivo.good()) 	// si todo sigui bien
	  // leo la frecuencia
	  archivo.read((char*)&(r->frecuencia), sizeof(r->frecuencia));
     else{
	  // si no, salgo
	  delete r;
	  return NULL;
     }

     // la frecuencia me indica la cantidad de punteros que debo leer.
     uint32_t contador = r->frecuencia;
     Registro::Punteros p;
     
     if(!compresion){
	  // si no estan comprimidos, lo leo directamente.
	  while(archivo.good() && contador > 0){
	       // leo documento
	       archivo.read((char*)&(p.documento), sizeof(p.documento));
	       // leo frecuencia
	       archivo.read((char*)&(p.frecuencia), sizeof(p.frecuencia));
	       // agregoel puntero
	       r->punteros.push_back(p);
	       // y paso al siguiente
	       contador--;
	  }
	  
     }
     else{
	  // si esta comprimido
	  char byte=0;
	  unsigned bit=1<<7;
	  uint32_t docAnterior = (uint32_t)-1;
	  unsigned bits=0;
	  unsigned indice=0;
	  uint32_t valores[2];
	  std::string aux;

	  // hasta que lea la cantidad total
	  while(archivo.good() && contador > 0){
	       // si me quede sin bits (o recien empiezo)
	       if(bits==0){
		    // leo un byte
		    archivo.read(&byte, 1);
		    // me quedan 8 bits por procesar
		    bits=8;
		    // proximo bit a procesar
		    bit=1<<7;
	       }

	       // mientras me queden bits por procesar y no haya
	       // conseguido los 2 punteros que quiero
	       for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
		    // agregoun '1' o '0' a la cadena segun corresponda.
		    aux += ((byte)&(bit))>0?'1':'0';
		    
		    // intento decodificar el numero que tengo
		    if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1){
			 // si se pudo, aumento el indice
			 indice++;
			 // y vacio la cadena para empezar a
			 // decodificar el siguiente.
			 aux.clear();
		    }
	       }
	       // si ya decodifique los 2 punteros que quería
	       if(indice==2){
		    // asigno numero de documento (son distancias, por
		    // eso sumo el anterior).
		    docAnterior = p.documento = valores[0]+docAnterior;
		    // asigno la frecuencia
		    p.frecuencia=valores[1];
		    // agrego los punteros.
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

     frecuencia = punteros.size();

     // escribo el termino
     archivo.write(termino.c_str(), termino.length()+1); //+1 para que escriba el \0
     // escribo la frecuencia
     archivo.write((char*)&frecuencia, sizeof(frecuencia));

     
     std::list<Registro::Punteros>::iterator it;
     // si no se usa compresion
     if(compresion==0){
	  // escribo todos los punteros
	  for(it=punteros.begin(); it != punteros.end(); it++){
	       Registro::Punteros p;
	       p = *it;
	       // documento
	       archivo.write((char*)&(p.documento), sizeof(p.documento));
	       // frecuencia
	       archivo.write((char*)&(p.frecuencia), sizeof(p.frecuencia));
	  }
     }
     else{
	  // escribo comprimido
	  std::string punteros = obtenerPunterosComprimidos();
	  archivo.write((char*)punteros.c_str(), punteros.size());
     }
     return 1;
}

std::list<uint32_t> Registro::obtenerDocumentos(){
     std::list<Registro::Punteros>::iterator it;
     std::list<uint32_t> docs;
     for(it=punteros.begin();it!=punteros.end();it++){
	  docs.push_back((*it).documento);
     }
     return docs;
}

std::string Registro::obtenerPunterosComprimidos(){
     std::list<Registro::Punteros>::iterator it;
     const char* ptr;
     std::string str1, str2;
     char byte=0;
     unsigned bit=1<<7;
     uint32_t docAnterior = (uint32_t) -1;
     unsigned bits=0;
     
     std::string resultado;

     // recorro todos los punteros
     for(it=punteros.begin(); it != punteros.end(); it++){
	  Registro::Punteros p;
	  p = *it;
	  
	  // convierto el puntero a distancia y despues a GAMMA
	  str1= TDA_Codigos::getCGamma(p.documento-docAnterior);
	  docAnterior=p.documento; // almaceno el documento anterior

	  // Convierto la frecuencia a GAMMA
	  str2=TDA_Codigos::getCGamma(p.frecuencia);

	  // concateno ambos codigos
	  str1+=str2;
	  
	  ptr  = str1.c_str();
	  bits = str1.length();
	  while(bits > 0){
	       // escribo de a bits
	       for(;bit!=0 && bits > 0; bit >>= 1, bits--, ptr++){
		    byte |= *ptr!='0'?bit:0;
	       }
	       // si termine el byte, lo escribo a disco y paso al
	       // siguiente
	       if(bit==0){
		    // voy almacenando el resultado
		    resultado+=byte;
		    bit=1<<7;
		    byte=0;
	       }
	  }
     }
     // si me quedo algun byte por la mitad, lo escribo asi
     if(bit != 1<<7){
	  resultado += byte;
     }
     
     // devuelvo el resultado
     return resultado;
}

uint32_t Registro::obtenerFrecuencia(){
     return punteros.size();
     return frecuencia;
}

const std::string& Registro::obtenerTermino(){
     return termino;
}

int Registro::unir(const Registro& registro){
     std::list<Registro::Punteros> final;
     std::list<Registro::Punteros>::iterator it1;
     std::list<Registro::Punteros>::const_iterator it2;

     // if(termino != registro.termino)
     // 	  return 0;

     // if(punteros.size() == 0)
     // 	  return 0;

     // if(registro.punteros.size() == 0)
     // 	  return 0;
     
     it1 = punteros.begin();
     it2 = registro.punteros.begin();


     // recorro las 2 listas de punteros
     while(it1!= punteros.end() && it2 != registro.punteros.end()){
	  // si la primera es menor que la segunda
	  if((*it1).documento < (*it2).documento){
	       // escribo la primera y avanzo
	       it1++;
	  }
	  // si la segunda es menor que la primera
	  else if((*it1).documento > (*it2).documento){
	       // escribo la segunda y avanzo
	       punteros.insert(it1, *it2);
	       it2++;
	       // aumento la frecuencia
	       frecuencia++;

	  }
	  else{
	       // si son iguales, sumo las frecuencias y avanzo en las
	       // dos listas
	       (*it1).frecuencia += (*it2).frecuencia ;
	       it1++;
	       it2++;
	  }

     }
     
     // termino de procesar los elementos que puedan quedar en la
     // lista
     while(it2!= registro.punteros.end()){
	  punteros.insert(it1,*it2);
	  it2++;
	  frecuencia++;
     }

     frecuencia = punteros.size();

     return 1;
}

void Registro::obtenerPunterosEnLista(std::ifstream& archivo, uint32_t offset, uint32_t frec, std::list<uint32_t>* lista_punteros) {

	char byte=0;
	unsigned bit=1<<7;
	uint32_t docAnterior = (uint32_t)-1;
	unsigned bits=0;
	unsigned indice=0;
	uint32_t valores[2];
	std::string aux;

	archivo.seekg(offset);

	while(archivo.good() && frec > 0){
	   if(bits==0){
		archivo.read(&byte, 1);
		bits=8;
		bit=1<<7;
	   }

	   for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
		aux += ((byte)&(bit))>0?'1':'0';
		
		if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1){
		     indice++;
		     aux.clear();
		}
	   }
	   if(indice==2){
		docAnterior = valores[0]+docAnterior;
		lista_punteros->push_back(docAnterior);
		indice=0;
		frec--;
	   }
	}
}

void Registro::obtenerPunterosEnLista(std::ifstream& archivo, uint32_t offset, uint32_t frec, std::list<Registro::Punteros>* lista_punteros) {

	char byte=0;
	unsigned bit=1<<7;
	uint32_t docAnterior = (uint32_t)-1;
	unsigned bits=0;
	unsigned indice=0;
	uint32_t valores[2];
	std::string aux;

	archivo.seekg(offset);


	while(archivo.good() && frec > 0){
	   if(bits==0){
		archivo.read(&byte, 1);
		bits=8;
		bit=1<<7;
	   }

	   for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
		aux += ((byte)&(bit))>0?'1':'0';
		
		if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1){
		     indice++;
		     aux.clear();
		}
	   }
	   if(indice==2){
		Punteros p;
		docAnterior = valores[0]+docAnterior;
		p.documento = docAnterior;
		p.frecuencia = valores[1];
		lista_punteros->push_back(p);
		indice=0;
		frec--;
	   }
	}
}

