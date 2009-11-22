#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "../Registros/Registro.h"
#include "../Util/Util.h"
#include "../Arbol/RedBlackTree.h"
#include "../FileManager/ConstPath.h"
#include <stdint.h>
/** 
 * Interfaz a implementar por cada uno de los parsers del programa.
 */

class Parser{
	
private:	

     class StopWord{
     private:
	  std::string texto;
	  int cantidad;
	     
     public:
	  StopWord(std::string texto){
	       this->texto = texto;
	       cantidad = 1;
	  }
	  StopWord(){}
	  bool operator>(const StopWord& b){
	       return texto.compare(b.texto)>0?1:0;
	  }
	  bool operator<(const StopWord& b){
	       return texto.compare(b.texto)<0?1:0;
	  }
	  void unir(const StopWord& otro){
	       cantidad++;
	  }
	  const std::string print() const{
	       if(cantidad==1)
		    return texto;
	       else{
		    std::string s;
		    std::stringstream z;
		    z << texto << "(" << cantidad << ")";
		    z >> s;
		    return s;
	       }
	  }
     };

     RedBlackTree<StopWord> arbol;

protected:
     std::string nombreCatalogo;
     std::string nombreBase;
     uint32_t archivos;
     uint32_t cantMaxReg;
     uint32_t cantReg;
     std::list<std::string> lista;

     /**
      * La funcion se encarga de cargar el arbol con los stop words.
      * 
      * @param nombreStopWord El archivo donde estan los stop words.
      */ 
     void cargarStopWord(const std::string& nombreStopWord);

     /**
      * La funcion se encarga de decir si la palabra pasada por
      * parametro es o no stop word. Devuelve TRUE si es stop word,
      * FALSE caso contrario.
      * 
      * @param palabra La palabra a saber si es o no stop word.
      */ 
     bool esStopWord(const std::string& palabra);
	
     /** 
      * Transforma un string a minusculas.
      * 
      * @param str El string a transformar.
      */
     void aMinusculas(std::string& str);
		
     /**
      * La funcion toma una cadena, filtra los espacios y guarda palabra a
      * palabra, en una lista auxiliar.
      */ 
     void guardarPalabras(const std::string& palabras);

public:
     /** 
      * Crea un nuevo Parser, con un numero maximo de registros
      * por archivo.
      * 
      * @param cantMaxReg La cantidad maxima de registros por archivo.
      */
     Parser(uint32_t cantMaxReg);
		 
     /** 
      * Destruye el Parser.
      */
     virtual ~Parser();
		
     /** 
      * El método intenta abrir el archivo con el nombre dado e
      * intenta parsearlo. Si puede, lo hace y vuelca los registros
      * resultantes en un archivo dump. Cada archivo dump, tiene
      * una cantidad maxima de registros que puede almacenar, el cual
      * es dado por parametro en el constructor. Si se supera esa
      * cant maxima se crea otro archivo dump. El formato de las
      * ruta de los archivos dump es: dump_tipoParser_#archivo
      *
      * @param nombre El nombre del archivo a parsear.
      * @param documento El numero de documento.
      * @return TRUE si pudo realizar el parseo, FALSE caso contrario.
      * 
      * @see Registro
      */
     virtual bool parsear(std::string nombre, uint32_t documento)=0;

     /** 
      * Devuelve la cantidad de archivos que genero parseando.
      */     
     uint32_t getCantArchivosParseados();
     
     /** 
      * Devuelve la cantidad de registros del ultimo archivo.
      */     
     uint32_t getCantReg();			

     /** 
      * Verifica que la extension del archivo sea alguna de las
      * contenidas en el array (terminado en NULL)
      * 
      * @param nombre El nombre del archivo
      * @param char Lista de extensiones terminada en NULL.
      * 
      * @return TRUE si la extension concuerda, FASE si no.
      */
     bool verificarExtension(const std::string& nombre, const char **validas);


     /**
      * La funcion devuelve una cadena en minuscula, sin caracteres invalidos;
      * Notar que los caracteres invalidos son convertidos en espacios, por lo
      * que puede devolverse una cadena con mas de una palabra o una cadena
      * vacia.
      */
    static std::string aMinuscSinInvalidos(const std::string& informacion);

     const std::string& getNombreCatalogo(){ return nombreCatalogo; }
     const std::string& getNombreBase(){ return nombreBase; }

     virtual void flush(){ }

};

#endif //PARSER_H_INCLUDED
