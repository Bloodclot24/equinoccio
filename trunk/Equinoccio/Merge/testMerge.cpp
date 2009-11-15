#include "Merge.h"
#include "../Registros/Registro.h"

int main(int argc, char** argv){
     if(argc < 3)
	  return -1;
     
     std::vector<std::string> particiones;
     for(int i=1;i<argc;i++)
	  particiones.push_back(argv[i]);
     
     Merger<Registro>::Merge(particiones, "Archivosalida.merged");

     return 0;
}

