#include "ParserCPHP.h"
int main(int argc, char** argv){/*Main de Prueba*/
	std::string aux ="";
	ParserCPHP	 parser(500);

	//parser.parsear("TestPhp/test15.php",0);//OK
	//parser.parsear("TestC/test18.H",0);//OK
	parser.parsear("TestC/test10",0);//OK
	std::cout << "Finalizado" << std::endl;
	return 0;
}
