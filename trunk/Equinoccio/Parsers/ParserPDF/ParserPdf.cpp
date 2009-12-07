#include "ParserPdf.h"
#include <iostream>
#include "../libExtractor/libExtractor.h"
#include "../../FileManager/FileManager.h"

ParserPdf::ParserPdf(uint32_t cantMaxReg):Parser(cantMaxReg) {
     nombreCatalogo = "TXT";
     nombreBase+= PDF_DUMP_BASENAME;
     cargarStopWord(PDF_STOP_WORD_FILE);
}

ParserPdf::~ParserPdf() {}

bool ParserPdf::parsear(std::string nombre, uint32_t documento) {

     const char* validas[]={".pdf",0};
     if(!verificarExtension(nombre,validas))
	  return false;

	libExtractor extractor;
	EXTRACTOR_KeywordList *keywords = extractor.getKeyword(nombre);

	bool is_ok = (keywords != NULL);

	if (is_ok) {

		int keyType;
		std::string keyword;

		do {
			keyType = extractor.getKeywordType(keywords);
			keyword = extractor.getKeyword(keywords);

			if (keyType == PDF_FILENAME || keyType == PDF_TITLE || keyType == PDF_AUTHOR
					|| keyType == PDF_PUBLISHER || keyType == PDF_SUBJECT || keyType == PDF_KEYS
					|| keyType == PDF_CREATOR || keyType == PDF_PRODUCER) {

				guardarPalabras( aMinuscSinInvalidos(keyword) );
			}

			if (keyType == PDF_MIMETYPE) {
				if (keyword != MIME_TYPE_PDF)
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

		if (is_ok) {
		     // siempre parseo en elultimo segmento
			std::string dump_name =  FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
			dump_name += PDF_DUMP_BASENAME;
			dump_name += Util::intToString(archivos);

			std::ofstream dump;
			dump.open(dump_name.c_str(), std::ofstream::out | std::ofstream::app);

			/* guardo la lista de palabras en el dump */
			std::list<std::string>::iterator it;
			for (it = lista.begin(); it != lista.end(); it++) {

				if (cantReg != cantMaxReg) {
					guardarEnDump(dump, *it, documento);
					cantReg++;
				}
				else {
					cantReg = 1;
					archivos++;
					dump.close();
					// siempre parseo en el ultimo segmento
					dump_name =  FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
					dump_name += PDF_DUMP_BASENAME;
					dump_name += Util::intToString(archivos);
					dump.open(dump_name.c_str(), std::ofstream::out);
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

void ParserPdf::guardarEnDump(std::ofstream& dump, std::string& palabra, uint32_t documento) {

	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
}
