#include "ParserAudio.h"

/****************************************************************************/
std::string ParserAudio::parsear(std::string nombre, uint32_t documento) {
	
	EXTRACTOR_ExtractorList *extractors =EXTRACTOR_loadDefaultLibraries();
  
  //Funcion de la libreria extractor que guarda todos los contenidos
  //de lo diferentes campos en una estructura
  /*
   * typedef struct EXTRACTOR_Keywords {
   *	char* keyword;
   *	EXTRACTOR_KeywordType keywordType;
   *	struct EXTRACTOR_Keywords* next;
   * } EXTRACTOR_KeywordList;
   */
  EXTRACTOR_KeywordList *keywords= EXTRACTOR_getKeywords(extractors, nombre.c_str());
  //EXTRACTOR_printKeywords(stdout, keywords);
  
  std::string nombre_dump("");
  
  if(keywords) {
	  
	  bool audio= true;
	  
		do {
			int type= keywords->keywordType;
			
			if(type == FILENAME || type == MIMETYPE || type == TITLE || 
				 type == AUTHOR || type == ARTIST || type == DATE || type == PUBLISHER 
				 || type == LANGUAGE || type == ALBUM || type == GENRE ||type == YEAR 
				 || type == TRACK_NUMBER || type == 6) {
	  			
	  			//Si se trata del anio o el numero de cancion lo agregamos a
	  			//la lista. 
	  			//Si se trata del tipo de archivo, se checkea que sea de audio
	  			//y se guarga la extension.
	  			//En otro caso, se debe parsear las palabras y se agregan
	  			//una a una. 
	  			if(type == DATE || type == YEAR || type == TRACK_NUMBER)
	  				lista.push_back(keywords->keyword);
	  			else if(type == MIMETYPE) {
	  				if(strcmp(MIME_TYPE_MP3, keywords->keyword) != 0 && 
	  				   strcmp(MIME_TYPE_OGG, keywords->keyword) != 0)
	  					audio= false;
	  				else
	  					guardarExtension(keywords->keyword);	
	  			} else
	  				guardarPalabras(keywords->keyword);
			}
	  	
	  	keywords= keywords->next;
		} while(keywords != NULL);
	  
	  EXTRACTOR_freeKeywords(keywords);
		EXTRACTOR_removeAll(extractors);
	  
	  if(audio) {
		  			
			//Ruta dump
			nombre_dump+= nombre;
			nombre_dump+= PATH_DUMP;
			std::ofstream dump(nombre_dump.c_str(), std::fstream::out);
		
			//Guardo en el archivo dump
			std::list<std::string>::iterator it;
			for(it= lista.begin(); it != lista.end(); it++) {
				//TODO: FALTA CHECKEO STOP WORD Y CARACTERES INVALIDOS
				if(strcspn((*it).c_str(), "0123456789~!@#$%^&*()_+{}|:?><<>[],.-") == (*it).length()) {  	
		  		//std::cout << "Palabra a filtrar: " << *it << std::endl;
		  		//filtrarPalabra(*it);
		  		Registro reg(*it, documento);
		  		std::cout << "Palabra a escribir: " << reg.obtenerTermino() << std::endl;
		  		reg.escribir(dump, 0);
				}
			}	
			
			dump.close();
		}
  }
   
  return nombre_dump;
}

/*--------------------------------------------------------------------------*/
void ParserAudio::guardarExtension(char* extension) {

	std::string strAparsear(extension); 
	size_t found= strAparsear.find("/", 0);
	std::string palabraAgregar(strAparsear, found+1);
	lista.push_back(palabraAgregar);	
}

/*--------------------------------------------------------------------------*/
void ParserAudio::guardarPalabras(char* palabras) {
	
	std::string strAparsear(palabras); 
	size_t found, pos= 0;
	
	do {
		found= strAparsear.find(" ", pos);
		std::string palabraAgregar(strAparsear, pos, found-pos);
		lista.push_back(palabraAgregar);	
		pos= found+1;
	
	} while(found != std::string::npos);
}

/****************************************************************************/