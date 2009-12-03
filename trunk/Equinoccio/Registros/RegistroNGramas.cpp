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
     uint32_t docAnterior = 0;
     unsigned bits=0;
     std::string resultado;
     std::list<uint32_t>::iterator it;     
     for(it=punteros.begin(); it != punteros.end(); it++){
	  uint32_t p;
	  p = *it;
	  
	  str = TDA_Codigos::getCGamma(p-docAnterior+1);
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
     std::list<uint32_t> final;
     std::list<uint32_t>::const_iterator it1, it2;

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
	  if((*it1) < (*it2)){
	       final.push_back(*it1);
	       it1++;
	  }
	  else if((*it1) > (*it2)){
	       final.push_back(*it2);
	       it2++;
	  }
	  else{
	       final.push_back(*it1);
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

	   for(;bit!=0 && bits > 0 && indice < 2; bit >>= 1, bits--){
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
