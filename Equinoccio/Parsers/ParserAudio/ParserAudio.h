#ifndef PARSERAUDIO_H_
#define PARSERAUDIO_H_

#include <stdio.h>
#include <list>
#include <extractor.h>
#include <fstream>
#include <sstream>
#include "../Parser.h"

// Los campos de los archivos de audio que se van a tener en cuenta
enum KeywordType {
	FILENAME = 1,
	MIMETYPE = 2,
  TITLE = 3,
	AUTHOR = 4,
  ARTIST = 5,
  DATE = 8,
  PUBLISHER = 9,
  LANGUAGE = 10,
  ALBUM = 11,
  GENRE = 12,
  YEAR = 115,
  TRACK_NUMBER = 132,
};

#define PATH_STOP_WORDS "stopword"
#define MIME_TYPE_MP3 "audio/mpeg"
#define MIME_TYPE_OGG "application/ogg"

/****************************************************************************/
class ParserAudio: public Parser {
	
	private:
		void guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento);
		std::string obtenerExtension(char* extension);
	
	public: 
		virtual std::string parsear(std::string nombre, uint32_t documento);	
};

/****************************************************************************/
#endif /*PARSERAUDIO_H_*/