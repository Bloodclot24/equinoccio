#include "ParserAudio.h"

/****************************************************************************/
std::string ParserAudio::parsear(std::string nombre, uint32_t documento) {
	
	EXTRACTOR_ExtractorList *extractors =EXTRACTOR_loadDefaultLibraries();
  EXTRACTOR_KeywordList *keywords= EXTRACTOR_getKeywords(extractors, nombre.c_str());
  
  //Ruta dump
  std::string nombre_dump;
	nombre_dump+= nombre;
	nombre_dump+= PATH_DUMP;
	std::ofstream dump(nombre_dump.c_str(), std::fstream::out);
  
  bool audio= true;
  
  if(keywords) {
	  
	  do {
			int type= keywords->keywordType;
			
			if(type == FILENAME || type == MIMETYPE || type == TITLE || 
				 type == AUTHOR || type == ARTIST || type == DATE || type == PUBLISHER 
				 || type == LANGUAGE || type == ALBUM || type == GENRE ||type == YEAR 
				 || type == TRACK_NUMBER || type == 6) {
	  			
	  			//Si se trata del anio o el numero de cancion lo agregamos a
	  			//la lista directamente. 
	  			//Si se trata del tipo de archivo, se checkea que sea de audio
	  			//y se guarga la extension.
	  			//En otro caso, se debe parsear las palabras y se agregan
	  			//una a una. 
	  			if(type == DATE || type == YEAR || type == TRACK_NUMBER) {
	  				guardarEnDump(dump, keywords->keyword, documento);
			  		//TODO: /*PRUEBA*/
			  		std::cout << "Palabra A Guardar: " << keywords->keyword << std::endl;
	  			} else if(type == MIMETYPE) {

	  				if(strcmp(MIME_TYPE_MP3, keywords->keyword) != 0 && 
	  				   strcmp(MIME_TYPE_OGG, keywords->keyword) != 0)
	  					audio= false;
	  				else {
	  					std::string extension= obtenerExtension(keywords->keyword); 
	  					guardarEnDump(dump, extension, documento);
				  		//TODO: /*PRUEBA*/
	  					std::cout << "Palabra A Guardar: " << extension << std::endl;
	  				}	

	  			} else
							guardarPalabras(aMinuscSinInvalidos(keywords->keyword));
			}
	  	
	  	keywords= keywords->next;
		} while(keywords != NULL);
	  
	  EXTRACTOR_freeKeywords(keywords);
		EXTRACTOR_removeAll(extractors);
	  
	  if(audio) {
			//Guardo en el archivo dump
			std::list<std::string>::iterator it;
			for(it= lista.begin(); it != lista.end(); it++) {
					//TODO: FALTA CHECKEAR ANTES DE QUE NO SEA STOP WORD
		  		//TODO: /*PRUEBA*/
					std::cout << "Palabra A Guardar: " << *it << std::endl;
					guardarEnDump(dump, (*it), documento);
			}
		}
  }
  
  dump.close();
  
  if(!audio)
  	nombre_dump= "";
   
  return nombre_dump;
}

/*--------------------------------------------------------------------------*/
void ParserAudio::guardarEnDump(std::ofstream& dump, 
																std::string palabra, uint32_t documento) {
	
	Registro reg(palabra, documento);
	reg.escribir(dump, 0);
}

/*--------------------------------------------------------------------------*/
std::string ParserAudio::obtenerExtension(char* extension) {

	std::string strAparsear(extension); 
	size_t found= strAparsear.find("/", 0);
	std::string palabraExtension(strAparsear, found+1);

	return palabraExtension;	
}

/****************************************************************************/