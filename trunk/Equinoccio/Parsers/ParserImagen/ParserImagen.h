#ifndef PARSERIMAGEN_H_
#define PARSERIMAGEN_H_

#include "../Parser.h"

#define MIME_TYPE_JPEG "image/jpeg"
#define MIME_TYPE_PNG "image/png"
#define DUMP_NAME "dump_img_"
#define PATH_STOPWORDS "stop_words_img"

enum KeywordType {
	FILENAME = 1,
	MIME_TYPE = 2,
	DESCRIPCION = 6,
	COMMENT = 7,
	SOFTWARE = 26,
	CAMERA_MAKE = 72,
	CAMERA_MODEL = 73
};


class ParserImagen: public Parser {

private:
	void guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento);

public:
	ParserImagen(uint32_t cantMaxReg);
	~ParserImagen();


	bool parsear(std::string nombre, uint32_t documento);
};

#endif /* PARSERIMAGEN_H_ */
