#ifndef PARSERAUDIO_H_
#define PARSERAUDIO_H_

#include <stdio.h>
#include <list>
#include "../libExtractor/libExtractor.h" 
#include "../Parser.h"

// Los campos de los archivos de audio que se van a tener en cuenta
enum AUDIO_KeywordType {
     AUDIO_FILENAME = 1,
     AUDIO_MIMETYPE = 2,
     AUDIO_TITLE = 3,
     AUDIO_AUTHOR = 4,
     AUDIO_ARTIST = 5,
     AUDIO_DATE = 8,
     AUDIO_PUBLISHER = 9,
     AUDIO_LANGUAGE = 10,
     AUDIO_ALBUM = 11,
     AUDIO_GENRE = 12,
     AUDIO_YEAR = 115,
     AUDIO_TRACK_NUMBER = 132,
};

#define MIME_TYPE_MP3 "audio/mpeg"
#define MIME_TYPE_OGG "application/ogg"

/****************************************************************************/
class ParserAudio: public Parser {
	
	private:
		void guardarEnDump(std::ofstream& dump, std::string& palabra, uint32_t documento);
	
	public: 
		ParserAudio(uint32_t cantMaxReg);
		virtual bool parsear(std::string nombre, uint32_t documento);	
};

/****************************************************************************/
#endif /*PARSERAUDIO_H_*/
