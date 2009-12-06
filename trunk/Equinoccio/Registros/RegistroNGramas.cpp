#include "RegistroNGramas.h"
#include "Registro.h"

RegistroNGramas::RegistroNGramas(const std::string& termino, uint32_t documento){
     this->termino = termino;
     frecuencia = 1;
     punteros.push_back(documento);
}

RegistroNGramas* RegistroNGramas::leer(std::ifstream &archivo, int compresion){
     RegistroNGramas* r= new RegistroNGramas();
     char c=-1;
     unsigned i=0;
     while(archivo.good() && (i < 2)){
	  c = archivo.get();
	  r->termino += c;
	  i++;
     }

     if(archivo.good())
	  archivo.read((char*)&(r->frecuencia), sizeof(r->frecuencia));
     else{
	  delete r;
	  return NULL;
     }

     uint32_t contador = r->frecuencia;
     uint32_t doc;
     if(!compresion){
	  while(archivo.good() && contador > 0){
	       archivo.read((char*)&(doc), sizeof(doc));
	       r->punteros.push_back(doc);
	       contador--;
	  }
     }
     else{

	  char byte=0;
	  unsigned bit=1<<7;
	  uint32_t docAnterior = 0;
	  unsigned bits=0;
	  bool salir=false;;
	  uint32_t doc;
	  std::string aux;

	  while(archivo.good() && contador > 0){
	       if(bits==0){
		    archivo.read(&byte, 1);
		    bits=8;
		    bit=1<<7;
	       }

	       for(;bit!=0 && bits > 0 && !salir; bit >>= 1, bits--){
		    aux += ((byte)&(bit))>0?'1':'0';
		    
		    if((doc = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1)
			 salir=true;
	       }
	       if(salir){
		    salir=false;
		    docAnterior = doc+docAnterior;
		    r->punteros.push_back(docAnterior);
		    contador--;
	       }
	  }
     }
     
     return r;
}

int RegistroNGramas::generarEscribir(std::ofstream &archivo, int compresion, Registro& r, uint32_t offset){
     RegistroNGramas* reg= new RegistroNGramas();
     std::string termino = "$";
     unsigned contador=0;
     termino += r.obtenerTermino() + "$";
     reg->frecuencia = 1; 
     reg->punteros.push_back(offset);
     for(unsigned i=0;i<termino.size()-1;i++){
	  reg->termino = termino.substr(i, 2);
	  reg->escribir(archivo, compresion);
	  contador++;
     }

     delete reg;
     
     return contador;
}

int RegistroNGramas::escribir(std::ofstream &archivo, int compresion){
     if(!archivo.good())
	  return 0;
     if(punteros.size() == 0)
	  return 0;

     archivo.write(termino.c_str(), termino.length()); //no guarda el 0
     archivo.write((char*)&frecuencia, sizeof(frecuencia));

     std::list<uint32_t>::iterator it;
     if(compresion==0){
	  for(it=punteros.begin(); it != punteros.end(); it++){
	       uint32_t doc;
	       doc = *it;
	       archivo.write((char*)&(doc), sizeof(doc));
	  }
     }
     else{
	  std::string punteros = obtenerPunterosComprimidos();
	  archivo.write(punteros.c_str(), punteros.size());
     }
     return 1;
}


std::string RegistroNGramas::obtenerPunterosComprimidos(){
     const char* ptr;
     std::string str;
     char byte=0;
     unsigned bit=1<<7;
     uint32_t docAnterior = (uint32_t)-1;
     unsigned bits=0;
     std::string resultado;
     std::list<uint32_t>::iterator it;     
     for(it=punteros.begin(); it != punteros.end(); it++){
	  uint32_t p;
	  p = *it;
	  
	  str = TDA_Codigos::getCGamma(p-docAnterior);
	  docAnterior=p;
	  
	  ptr  = str.c_str();
	  bits = str.length();
	  while(bits > 0){
	       for(;bit!=0 && bits > 0; bit >>= 1, bits--, ptr++){
		    byte |= *ptr!='0'?bit:0;
	       }
	       if(bit==0){
		    /* escribir el byte a disco */
		    resultado += byte;
		    bit=1<<7;
		    byte=0;
	       }
	  }
     }
     if(bit != 1<<7){
	  resultado += byte;
     }
     return resultado;
}

uint32_t RegistroNGramas::obtenerFrecuencia(){
     return frecuencia;
}


const std::string& RegistroNGramas::obtenerTermino(){
     return termino;
}

int RegistroNGramas::unir(const RegistroNGramas& registro){
     std::list<uint32_t>::iterator it1;
     std::list<uint32_t>::const_iterator it2;

     if(termino != registro.termino)
	  return 0;

     if(punteros.size() == 0)
	  return 0;

     if(registro.punteros.size() == 0)
	  return 0;
     
     it1 = punteros.begin();
     it2 = registro.punteros.begin();


     // recorro las 2 listas de punteros
     while(it1!= punteros.end() && it2 != registro.punteros.end()){
	  // si la primera es menor que la segunda
	  if((*it1) < (*it2)){
	       // escribo la primera y avanzo
	       it1++;
	  }
	  // si la segunda es menor que la primera
	  else if((*it1) > (*it2)){
	       // escribo la segunda y avanzo
	       punteros.insert(it1, *it2);
	       it2++;
	       // aumento la frecuencia
	       frecuencia++;

	  }
	  else{
	       // si son iguales, sumo las frecuencias y avanzo en las
	       // dos listas
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

     
     return 1;
}

void RegistroNGramas::obtenerPunterosEnLista(std::ifstream& archivo, uint32_t offset, uint32_t frec, std::list<uint32_t>* lista_punteros) {

	char byte=0;
	unsigned bit=1<<7;
	uint32_t docAnterior = (uint32_t)-1;
	unsigned bits=0;
	unsigned indice=0;
	uint32_t valores[1];
	std::string aux;

	archivo.seekg(offset);

	while(archivo.good() && frec > 0){
	   if(bits==0){
		archivo.read(&byte, 1);
		bits=8;
		bit=1<<7;
	   }

	   for(;bit!=0 && bits > 0 && indice < 1; bit >>= 1, bits--){
		aux += ((byte)&(bit))>0?'1':'0';
		
		if((valores[indice] = TDA_Codigos::getNGamma(aux)) != (uint32_t)-1){
		     indice++;
		     aux.clear();
		}
	   }
	   if(indice==1){
		docAnterior = valores[0]+docAnterior;
		lista_punteros->push_back(docAnterior);
		indice=0;
		frec--;
	   }
	}
}
