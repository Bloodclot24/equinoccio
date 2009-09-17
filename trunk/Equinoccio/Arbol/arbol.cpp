#include <stdlib.h>
#include <sys/time.h>
#include <string>

class entradaTexto{
private:
     std::string texto;
     
public:
     entradaTexto(std::string texto){
	  this->texto = texto;
     }
     entradaTexto(){}
     bool operator>(const entradaTexto& b){
	  return texto.compare(b.texto)>0?1:0;
     }
     const std::string print() const{
	  return texto;
     }
};

#include "RedBlackTree.h"


#define CARACTERES 5

int main(int argc, char**argv){
     
     if(argc < 2){
	  std::cout << "La sintaxis es: " << argv[0] << " <n>" << std::endl;
     }
     RedBlackTree<entradaTexto> arbol;

     struct timeval t;
     gettimeofday(&t, NULL);
     srand(t.tv_usec);

     std::string inicial;

     for(int i=0;i<atoi(argv[1]);i++){
	  inicial.clear();
	  for(int j=0;j<CARACTERES;j++){
	       inicial += rand()%26+'A';
	  }
	  arbol.Insert(entradaTexto(inicial));
     }

     arbol.Print();

     return 0;
}
