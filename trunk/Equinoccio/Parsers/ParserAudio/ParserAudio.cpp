#include "ParserAudio.h"

/****************************************************************************/
ParserAudio::ParserAudio(uint32_t cantMaxReg): Parser::Parser(cantMaxReg) { 
	
	cargarStopWord(PATH_STOP_WORD);
}; 
		
/*--------------------------------------------------------------------------*/
bool ParserAudio::parsear(std::string nombre, uint32_t documento) {
	
	EXTRACTOR_ExtractorList *extractors =EXTRACTOR_loadDefaultLibraries();
  EXTRACTOR_KeywordList *keywords= EXTRACTOR_getKeywords(extractors, nombre.c_str());
  
  std::string nombre_dump;
  
  bool audio= false;
  
  if(keywords) {
		
		audio= true;

	  //Ruta dump
		nombre_dump+= PATH_DUMP_AUDIO;
		nombre_dump+= Util::intToString(archivos);
		std::ofstream dump;
		dump.open(nombre_dump.c_str(), std::fstream::out);

//		//TODO: /*PRUEBA*/
//		std::cout << "Ruta Dump: " << nombre_dump << std::endl;

	  
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
//			  		//TODO: /*PRUEBA*/
//			  		std::cout << "Palabra A Guardar: " << keywords->keyword << std::endl;
	  			} else if(type == MIMETYPE) {

	  				if(strcmp(MIME_TYPE_MP3, keywords->keyword) != 0 && 
	  				   strcmp(MIME_TYPE_OGG, keywords->keyword) != 0)
	  					audio= false;
	  				else {
	  					std::string extension= obtenerExtension(keywords->keyword); 
	  					guardarEnDump(dump, extension, documento);
				  		////TODO: /*PRUEBA*/
	  					//std::cout << "Palabra A Guardar: " << extension << std::endl;
	  				}	

	  			} else
							guardarPalabras(aMinuscSinInvalidos(keywords->keyword));
			}
	  	
	  	keywords= keywords->next;
		} while(keywords != NULL);
	  
	  if(audio) {
			//Guardo en el archivo dump
			std::list<std::string>::iterator it;
			for(it= lista.begin(); it != lista.end(); it++) {
					//TODO: FALTA CHECKEAR ANTES DE QUE NO SEA STOP WORD
		  		//TODO: /*PRUEBA*/
					if(cantReg != cantMaxReg) {
//						std::cout << "Palabra A Guardar: " << *it << std::endl;
						guardarEnDump(dump, (*it), documento);
						cantReg++;
					} else {
						dump.close();
						archivos++;
						nombre_dump.clear();
						nombre_dump+= PATH_DUMP_AUDIO;
						nombre_dump+= Util::intToString(archivos);
						dump.open(nombre_dump.c_str(), std::fstream::out);
						cantReg= 1;
//						//TODO: /*PRUEBA*/
//						std::cout << "-- Nuevo archivo dump --" << std::endl;
//						std::cout << "Ruta Dump: " << nombre_dump << std::endl;
//						std::cout << "Palabra A Guardar: " << *it << std::endl;
						guardarEnDump(dump, (*it), documento);
					}
			}
		}
		dump.close();
  }
  
//  std::cout << "Cant Archivos: " << archivos << std::endl;
//  std::cout << "Cant registros ultimo archivo: " << cantReg << std::endl;
  
 	EXTRACTOR_freeKeywords(keywords);
	EXTRACTOR_removeAll(extractors);
  lista.clear();
  
	return audio;
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