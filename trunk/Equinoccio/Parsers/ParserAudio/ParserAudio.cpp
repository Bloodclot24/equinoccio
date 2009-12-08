#include "ParserAudio.h"
#include "../../FileManager/FileManager.h"

/****************************************************************************/
ParserAudio::ParserAudio(uint32_t cantMaxReg): Parser::Parser(cantMaxReg) { 
     nombreCatalogo = "SND";
     nombreBase += AUDIO_DUMP_BASENAME;
     cargarStopWord(AUDIO_STOP_WORD_FILE);
}; 
		
/*--------------------------------------------------------------------------*/
bool ParserAudio::parsear(std::string nombre, uint32_t documento) {

	 const char* validas[]={".mp3",".ogg",0};
     if(!verificarExtension(nombre,validas))
	  return false;
  
     libExtractor extractor;
     EXTRACTOR_KeywordList *keywordList= extractor.getKeyword(nombre.c_str());
     EXTRACTOR_KeywordList *keywords= keywordList;
  
     // Siempre parseo en el ultimo segmento
     std::string nombre_dump = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     bool audio= false;
  
     if(keywords) {
		
	  audio= true;

	  //Ruta dump
	  nombre_dump+= AUDIO_DUMP_BASENAME;
	  nombre_dump+= Util::intToString(archivos);
	  std::ofstream dump;
	  dump.open(nombre_dump.c_str(), std::fstream::out | std::fstream::app);

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

		    } else if(type == AUDIO_MIMETYPE) {

			 if((keyword.compare(MIME_TYPE_MP3) != 0) && 
			    (keyword.compare(MIME_TYPE_OGG) != 0) && (keyword.compare(MIME_TYPE_WAV) != 0))
			      audio= false;
			 else {
			      unsigned int indexExt= nombre.find_last_of('.');
			      std::string ext;
			      if(indexExt != std::string::npos) {
			    	  ext= nombre.substr(indexExt + 1);
			    	  Util::aMinusculas(ext);
			    	  guardarEnDump(dump, ext, documento);
			      }
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
		    if(cantReg != cantMaxReg) {
			 guardarEnDump(dump, (*it), documento);
			 cantReg++;
		    } else {
			 dump.close();
			 archivos++;
			 nombre_dump.clear();
			 // Siempre parseo en el ultimo segmento
			 nombre_dump = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
			 nombre_dump+= AUDIO_DUMP_BASENAME;
			 nombre_dump+= Util::intToString(archivos);
			 dump.open(nombre_dump.c_str(), std::fstream::out);
			 cantReg= 1;
			 guardarEnDump(dump, (*it), documento);
		    }
	       }
	  }
	  dump.close();
     }

     lista.clear();
  
     return audio;
}

/*--------------------------------------------------------------------------*/
void ParserAudio::guardarEnDump(std::ofstream& dump, 
				std::string& palabra, uint32_t documento) {
	
     if(!esStopWord(palabra)) {
	  Registro reg(palabra, documento);
	  reg.escribir(dump, 0);
     }
}

/****************************************************************************/
