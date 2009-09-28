#include "ParserImagen.h"
#include <iostream>
#include "../libExtractor/libExtractor.h"


//TODO sacar los couts
ParserImagen::ParserImagen(uint32_t cantMaxReg):Parser(cantMaxReg) {

	cargarStopWord(PATH_STOPWORDS);
}

ParserImagen::~ParserImagen() {}

bool ParserImagen::parsear(std::string nombre, uint32_t documento) {

	libExtractor extractor;
	EXTRACTOR_KeywordList *keywords = extractor.getKeyword(nombre);

	bool is_ok = (keywords != NULL);

	if (is_ok) {

		int keyType;
		std::string keyword;

		do {
			keyType = extractor.getKeywordType(keywords);
			keyword = extractor.getKeyword(keywords);

			if (keyType == FILENAME || keyType == COMMENT || keyType == CAMERA_MODEL
					|| keyType == CAMERA_MAKE || keyType == SOFTWARE) {

				std::cout<<"KEY: "<<keyword<<std::endl;
				guardarPalabras( aMinuscSinInvalidos(keyword) );

			}

			if (keyType == MIME_TYPE) {
				std::cout<<"mime: "<<keyword<<std::endl;
				if (keyword != MIME_TYPE_JPEG && keyword != MIME_TYPE_PNG)
					is_ok = false;

				else {
					is_ok = true;

					/* guardo el nombre del archivo */
					unsigned int index = nombre.find_last_of('/');
					if (index != std::string::npos)
						guardarPalabras(aMinuscSinInvalidos( nombre.substr(index + 1) ));
					else
						guardarPalabras( aMinuscSinInvalidos(nombre) );
				}
			}

			keywords= extractor.getNext(keywords);
		}while(keywords != NULL);

		std::cout<<"endKeys"<<std::endl<<std::endl;

		if (is_ok) {
			std::string dump_name = DUMP_NAME;
			dump_name += Util::intToString(archivos);

			std::ofstream dump;
			dump.open(dump_name.c_str(), std::ofstream::out);

			/* guardo la lista de palabras en el dump */
			std::list<std::string>::iterator it;
			for (it = lista.begin(); it != lista.end(); it++) {

				if (cantReg != cantMaxReg) {
					std::cout << "Palabra A Guardar: " << *it << std::endl;
					guardarEnDump(dump, *it, documento);
					cantReg++;
				}
				else {
					cantReg = 1;
					archivos++;
					dump.close();
					dump_name = DUMP_NAME;
					dump_name += Util::intToString(archivos);
					dump.open(dump_name.c_str(), std::ofstream::out);

					std::cout << "Palabra A Guardar: " << *it << std::endl;
					guardarEnDump(dump, *it, documento);
				}
			}
			dump.close();
		}
		lista.clear();
	}

	extractor.freeKeywords(keywords);
	return is_ok;
}

void ParserImagen::guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento) {

	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
	else
		std::cout<<"Stopword: "<<palabra<<std::endl;
}
