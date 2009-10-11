#include "libExtractor.h"

/*--------------------------------------------------------------------------*/
EXTRACTOR_KeywordList* libExtractor::getKeyword(std::string archivo) {
	
	extractors= EXTRACTOR_loadDefaultLibraries();
  EXTRACTOR_Keywords *keywords= EXTRACTOR_getKeywords(extractors, archivo.c_str());
  return keywords;
}

/*--------------------------------------------------------------------------*/
int libExtractor::getKeywordType(EXTRACTOR_Keywords* keywords) {
	
	return keywords->keywordType;
}

/*--------------------------------------------------------------------------*/
std::string libExtractor::getKeyword(EXTRACTOR_Keywords* keywords) {

	return keywords->keyword;	
}  	

/*--------------------------------------------------------------------------*/
EXTRACTOR_Keywords* libExtractor::getNext(EXTRACTOR_Keywords* keywords) {
	
	return keywords->next;
}

/*--------------------------------------------------------------------------*/
void libExtractor::freeKeywords(EXTRACTOR_KeywordList* keywordList) {
	
	EXTRACTOR_freeKeywords(keywordList);
	EXTRACTOR_removeAll(extractors);
}

/*--------------------------------------------------------------------------*/
