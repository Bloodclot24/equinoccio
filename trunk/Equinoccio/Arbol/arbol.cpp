#include "RedBlackTree.h"
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, char**argv){
     
     if(argc < 2){
	  std::cout << "La sintaxis es: " << argv[0] << " <n>" << std::endl;
     }
     RedBlackTree<int> arbol;

     struct timeval t;
     gettimeofday(&t, NULL);
     srand(t.tv_usec);

     for(int i=0;i<atoi(argv[1]);i++){
	  arbol.Insert(rand());
     }

     arbol.Print();

     return 0;
}
