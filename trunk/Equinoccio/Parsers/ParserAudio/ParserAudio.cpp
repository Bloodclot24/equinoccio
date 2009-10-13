#include "ParserAudio.h"

/****************************************************************************/
ParserAudio::ParserAudio(uint32_t cantMaxReg): Parser::Parser(cantMaxReg) { 
	
	cargarStopWord(AUDIO_PATH_STOP_WORD);
}; 
		
/*--------------------------------------------------------------------------*/
bool ParserAudio::parsear(std::string nombre, uint32_t documento) {
	
//	EXTRACTOR_ExtractorList *extractors =EXTRACTOR_loadDefaultLibraries();
//  EXTRACTOR_KeywordList *keywords= EXTRACTOR_getKeywords(extractors, nombre.c_str());
  
  libExtractor extractor;
  EXTRACTOR_KeywordList *keywordList= extractor.getKeyword(nombre.c_str());
  EXTRACTOR_KeywordList *keywords= keywordList;
  
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
			int type= extractor.getKeywordType(keywords);
			std::string keyword= extractor.getKeyword(keywords);
			
			if(type == AUDIO_FILENAME || type == AUDIO_MIMETYPE || type == AUDIO_TITLE || 
			   type == AUDIO_AUTHOR || type == AUDIO_ARTIST || type == AUDIO_DATE || 
			   type == AUDIO_PUBLISHER || type == AUDIO_LANGUAGE || type == AUDIO_ALBUM || 
			   type == AUDIO_GENRE ||type == AUDIO_YEAR || type == AUDIO_TRACK_NUMBER || 
			   type == 6) {
	  			
	  			//Si se trata del anio o el numero de cancion lo agregamos a
	  			//la lista directamente. 
	  			//Si se trata del tipo de archivo, se checkea que sea de audio
	  			//y se guarga la extension.
	  			//En otro caso, se debe parsear las palabras y se agregan
	  			//una a una. 
	  			if(type == AUDIO_DATE || type == AUDIO_YEAR || type == AUDIO_TRACK_NUMBER) {
	  				guardarEnDump(dump, keyword, documento);
//			  		//TODO: /*PRUEBA*/
//			  		std::cout << "Palabra A Guardar: " << keywords->keyword << std::endl;
	  			} else if(type == AUDIO_MIMETYPE) {

	  				if((keyword.compare(MIME_TYPE_MP3) != 0) && 
	  				   (keyword.compare(MIME_TYPE_OGG) != 0))
	  					audio= false;
	  				else {
	  					std::string extension= obtenerExtension(keyword); 
	  					guardarEnDump(dump, extension, documento);
				  		////TODO: /*PRUEBA*/
	  					//std::cout << "Palabra A Guardar: " << extension << std::endl;
	  				}	

	  			} else
							guardarPalabras(aMinuscSinInvalidos(keyword));
			}
	  	
	  	keywords= extractor.getNext(keywords);
		} while(keywords != NULL);
	  
	  extractor.freeKeywords(keywordList);
	  
	  if(audio) {
			//Guardo en el archivo dump
			std::list<std::string>::iterator it;
			for(it= lista.begin(); it != lista.end(); it++) {
					//TODO: /*PRUEBA*/
					if(cantReg != cantMaxReg) {
						//std::cout << "Palabra A Guardar: " << *it << std::endl;
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
						//TODO: /*PRUEBA*/
						//std::cout << "-- Nuevo archivo dump --" << std::endl;
						//std::cout << "Ruta Dump: " << nombre_dump << std::endl;
						//std::cout << "Palabra A Guardar: " << *it << std::endl;
						guardarEnDump(dump, (*it), documento);
					}
			}
		}
		dump.close();
  }
  
//  std::cout << "Cant Archivos: " << archivos << std::endl;
//  std::cout << "Cant registros ultimo archivo: " << cantReg << std::endl;
  
  lista.clear();
  
	return audio;
}

/*--------------------------------------------------------------------------*/
void ParserAudio::guardarEnDump(std::ofstream& dump, 
																std::string palabra, uint32_t documento) {
	
	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
}

/*--------------------------------------------------------------------------*/
std::string ParserAudio::obtenerExtension(std::string extension) {

	std::string strAparsear(extension); 
	size_t found= strAparsear.find("/", 0);
	std::string palabraExtension(strAparsear, found+1);

	return palabraExtension;	
}

/****************************************************************************/
