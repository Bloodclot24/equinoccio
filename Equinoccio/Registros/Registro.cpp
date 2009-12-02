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
	  // escribo comprimodo
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

     // *******************************************************************
     //    Devuelvo sin comprimir porque GAMMA no funciona del todo bien
     // *******************************************************************
     // recorro todos los punteros y los pongo en el string 
     for(it=punteros.begin(); it != punteros.end(); it++){
     	  Registro::Punteros p;
     	  p = *it;
     	  resultado.append((char*)&(p.documento),4);
     	  resultado.append((char*)&(p.frecuencia),4);
     	  std::cout << "obtengo: " << p.documento << " " << p.frecuencia << std::endl;
     }     

     std::cout << "Resultado: " << resultado << std::endl;
     
     return resultado;
     // ******************************************************************


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
     return frecuencia;
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
	       // **************************************************************
//	       p.frecuencia = 1; //TODO: OJO (para probar GAMMA)
	       // **************************************************************
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

void Registro::obtenerPunterosEnLista(std::ifstream& archivo, uint32_t offset, uint32_t frec, std::list<uint32_t>* lista_punteros) {

	char byte=0;
	unsigned bit=1<<7;
	uint32_t docAnterior = (uint32_t)-1;
	unsigned bits=0;
	unsigned indice=0;
	uint32_t valores[2];
	std::string aux;

	archivo.seekg(offset);

	while(frec-->0){
	     uint32_t auxi;
	     archivo.read((char*)&auxi, sizeof(uint32_t));
	     lista_punteros->push_back(auxi);
	     std::cout << "Documento: " << auxi << std::endl;
	     archivo.read((char*)&auxi, sizeof(uint32_t));
	}
	return;

	while(archivo.good() && frec > 0){
	   if(bits==0){
		archivo.read(&byte, 1);
		bits=8;
		bit=1<<7;
	   }
	   std::cout << "Distancia: ";
	   for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
		aux += ((byte)&(bit))>0?'1':'0';
		
		if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1){
		 indice++;
		 std::cout << aux << " ";
		 aux.clear();
		}
	   }
	   if(indice==2){
		docAnterior = valores[0]+docAnterior;
		std::cout << "\ndocumento: " << docAnterior << std::endl;
		lista_punteros->push_back(docAnterior);
		indice=0;
		frec--;
	   }
	}
}


