#include "RedBlackTree.h"
#include <stdlib.h>
#include <sys/time.h>

class enteros: public RedBlackEntry{
private:
     int key;
public:
     enteros(int clave):key(clave){};
     virtual int GetKey() const{
	  return key;
     }

     virtual void Print() const{
	  std::cout << key;
     }
};

int main(int argc, char**argv){
     
     if(argc < 2){
	  std::cout << "La sintaxis es: " << argv[0] << " <n>" << std::endl;
     }
     RedBlackTree arbol;

     struct timeval t;
     gettimeofday(&t, NULL);
     srand(t.tv_usec);

     for(int i=0;i<atoi(argv[1]);i++){
	  enteros *a = new enteros(rand());
	  arbol.Insert(a);
     }

     arbol.Print();

     return 0;
}
