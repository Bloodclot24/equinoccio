#ifndef PARSERPDF_H_
#define PARSERPDF_H_

#include "../Parser.h"
#include <iostream>

#define MIME_TYPE_PDF "application/pdf"

enum PDF_KeywordType {
	PDF_FILENAME = 1,
	PDF_MIMETYPE = 2,
	PDF_TITLE = 3,
	PDF_AUTHOR = 4,
	PDF_PUBLISHER = 9,
	PDF_SUBJECT = 17,
	PDF_KEYS = 18,
	PDF_CREATOR = 32,
	PDF_PRODUCER = 33,

};


class ParserPdf: public Parser {

private:
	void guardarEnDump(std::ofstream& dump, std::string& palabra, uint32_t documento);

public:

	ParserPdf(uint32_t cantMaxReg);
	~ParserPdf();


	bool parsear(std::string nombre, uint32_t documento);

};




#endif /*PARSERPDF_H_*/
