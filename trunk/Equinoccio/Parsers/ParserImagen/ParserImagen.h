#ifndef PARSERIMAGEN_H_
#define PARSERIMAGEN_H_

#include "../Parser.h"

#include <iostream>

#define MIME_TYPE_JPEG "image/jpeg"
#define MIME_TYPE_PNG "image/png"
#define IMAGEN_DUMP_NAME "dump_img_"
#define IMAGEN_PATH_STOPWORDS "stop_words_img"

enum IMG_KeywordType {
	IMG_FILENAME = 1,
	IMG_MIME_TYPE = 2,
	IMG_DESCRIPCION = 6,
	IMG_COMMENT = 7,
	IMG_SOFTWARE = 26,
	IMG_CAMERA_MAKE = 72,
	IMG_CAMERA_MODEL = 73
};


class ParserImagen: public Parser {

private:
     void guardarEnDump(std::ofstream& dump, std::string& palabra, uint32_t documento);

public:
	ParserImagen(uint32_t cantMaxReg);
	~ParserImagen();


	bool parsear(std::string nombre, uint32_t documento);
};

#endif /* PARSERIMAGEN_H_ */
