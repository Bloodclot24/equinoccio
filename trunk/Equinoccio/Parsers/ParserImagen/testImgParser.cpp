#include "ParserImagen.h"


int main() {

	ParserImagen p1(2000);
	p1.parsear("studio.jpg",3);

	ParserImagen p2(2);
	p2.parsear("heart_avi.png",3);

	ParserImagen p3(2000);
	p3.parsear("logo.png",3);
	return 0;
}
