#include "Sort.h"
#include "../Registros/Registro.h"

int main(int argc, char** argv){
     if(argc < 2)
	  return -1;

     Sorter<Registro>::Sort(argv[1], argv[1], 0, 50);

     return 0;
}
