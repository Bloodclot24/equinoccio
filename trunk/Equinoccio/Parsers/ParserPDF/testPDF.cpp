#include "ParserPdf.h"

int main (int argc,char** argv) {

	ParserPdf parser(10000);

	for (int i=1; i<argc; i++) {
		parser.parsear(argv[i], i);
	}

}



