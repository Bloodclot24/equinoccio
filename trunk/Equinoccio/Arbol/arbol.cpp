#include <stdlib.h>
#include <sys/time.h>
#include <string>
#include <sstream>

class entradaTexto{
private:
     std::string texto;
     int cantidad;
     
public:
     entradaTexto(std::string texto){
	  this->texto = texto;
	  cantidad = 1;
     }
     entradaTexto(){}
     bool operator>(const entradaTexto& b){
	  return texto.compare(b.texto)>0?1:0;
     }
     bool operator<(const entradaTexto& b){
	  return texto.compare(b.texto)<0?1:0;
     }
     void merge(const entradaTexto& otro){
	  cantidad++;
     }
     const std::string print() const{
	  if(cantidad==1)
	       return texto;
	  else{
	       std::string s;
	       std::stringstream z;
	       z << texto << "(" << cantidad << ")";
	       z >> s;
	       return s;
	  }
     }
};

#include "RedBlackTree.h"


#define CARACTERES 8

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
	  arbol.Insert(new entradaTexto(inicial));
     }

     std::cout << "Removiendo elementos >= 'I':\n";
     entradaTexto *E;
     while( (E = arbol.RemoverMayorIgual(entradaTexto("I"))) ){
	       std::cout << E->print()<<std::endl;
	       delete E;
     }

     while( (E = arbol.RemoverMayorIgual(entradaTexto(""))) ){
	       delete E;
     }

     return 0;
}

template <class N>
N* RedBlackTree<N>::RemoverMayorIgual(const N& referencia){
     RedBlackTreeNode<N> *x=root;
     RedBlackTreeNode<N> *y=root->left;
     RedBlackTreeNode<N> *ultimo_izquierda=NULL;
     
     while(y != nil){
	  x=y;
	  if(*(y->key) < referencia){
	       y=y->right;
	  }
	  else if(*(y->key) > referencia){
	       ultimo_izquierda = y;
	       y=y->left;
	  }
	  else{
	       ultimo_izquierda=y;
	       break;
	  }
     }
     
     if(ultimo_izquierda == NULL)
	  return NULL;
     return DeleteNode(ultimo_izquierda);
}
