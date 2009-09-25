#include <stdlib.h>
#include <sys/time.h>
#include <string>
#include <sstream>
#include "../Registros/Registro.h"

#include "RedBlackTree.h"


#define CARACTERES 1

int main(int argc, char**argv){
     
     if(argc < 2){
	  std::cout << "La sintaxis es: " << argv[0] << " <n>" << std::endl;
	  return -1;
     }
     RedBlackTree<Registro> arbol;

     struct timeval t;
     gettimeofday(&t, NULL);
     srand(t.tv_usec);

     std::string inicial;

     for(int i=0;i<atoi(argv[1]);i++){
	  inicial.clear();
	  for(int j=0;j<CARACTERES;j++){
	       inicial += rand()%26+'A';
	  }
	  Registro* R = new Registro(inicial, rand()%100);
	  if(arbol.Insert(R) == NULL)
	       delete R;
     }

     Registro *E;
     char clave[2];
     clave[1]=0;
     for(char c='A';c<='Z';c++){
	  clave[0]=c;
	  if(arbol.Search(Registro(clave,0)))
	       std::cout << "Clave " << c << " encontrada.\n";
	  else
	       std::cout << "Clave " << c << " NO encontrada.\n";
     }

     std::cout << "Removiendo registros >= 'I':\n";

     while( (E = arbol.RemoverMayorIgual(Registro("I",0))) ){
	       std::cout << E->print()<<std::endl;
	       delete E;
     }

     arbol.LiberarTodo(true);

     return 0;
}

