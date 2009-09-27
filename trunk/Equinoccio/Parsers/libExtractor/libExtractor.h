#ifndef LIBEXTRACTOR_H_
#define LIBEXTRACTOR_H_

#include "extractor.h"
#include <string>

/**
 * Encapsulamiento de la libreria extractor.
 */

class libExtractor {
	
	private:
		EXTRACTOR_ExtractorList *extractors;
	
	public:
		EXTRACTOR_KeywordList* getKeyword(std::string archivo);
		int getKeywordType(EXTRACTOR_Keywords* keywords);
		std::string getKeyword(EXTRACTOR_Keywords* keywords);  	
		EXTRACTOR_Keywords* getNext(EXTRACTOR_Keywords* keywords);
		void freeKeywords(EXTRACTOR_Keywords* keywordList);	  	
};

#endif /*LIBEXTRACTOR_H_*/
