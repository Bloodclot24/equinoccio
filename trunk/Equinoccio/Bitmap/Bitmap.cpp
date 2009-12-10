#include "Bitmap.h"

Bitmap::Bitmap(std::string nombreArchivo){
     cargado = false;
     bytes=0;
     buffer=NULL;
     archivo.open(nombreArchivo.c_str(), std::fstream::in | std::fstream::out);
     if(!archivo.good()){
	  archivo.open(nombreArchivo.c_str(), std::fstream::out| std::fstream::trunc);
	  archivo.close();
	  archivo.open(nombreArchivo.c_str(), std::fstream::in | std::fstream::out);
     }
}

Bitmap* Bitmap::cargar(std::string nombre){
     
     Bitmap *b=new Bitmap(nombre);
     b->cargado = true;
     b->archivo.seekg(0, std::ios_base::end);
     uint32_t bytes = b->archivo.tellg();
     b->bytes = bytes;
     if(bytes > 0){
	  b->buffer = new char[bytes];
	  b->archivo.seekg(0);
	  b->archivo.read(b->buffer, bytes);
     }

     return b;
}

void Bitmap::setBit(uint32_t bit, bool valor){
     char byte=0;
     
     if(!cargado){
	  if(archivo.good()){
	       archivo.seekp(0,std::ios_base::end);
	       uint32_t pos=archivo.tellp();
	       while(pos++ <= (bit/8)+((bit%8)>0?1:0))
		    archivo.write(&byte, 1);
	       archivo.seekg(bit/8);
	       archivo.read(&byte, 1);

	       uint32_t base= (valor==true)?1:0;
	       
	       byte &= ~(base<<(bit%8));
	       byte |= (base<<(bit%8));

	       archivo.seekp(bit/8);
	       archivo.write(&byte, 1);
	       archivo.clear();
	  }
     }
}

bool Bitmap::getBit(uint32_t bit){
     bool resultado=false;
     char byte=0;

     if(!cargado){
	  archivo.seekp(0,std::ios_base::end);
	  uint32_t pos=archivo.tellp();
	  
	  while(pos++ <= (bit/8)+((bit%8)>0?1:0))
	       archivo.write(&byte, 1);
	  
	  archivo.seekg(bit/8);
	  archivo.read(&byte, 1);
     }
     else {
	  if((bit/8)<bytes)
	       byte= buffer[bit/8];
	  else byte = 0;
     }

     resultado = (byte & (1<<(bit%8)))>0?1:0;
     return resultado;
}
