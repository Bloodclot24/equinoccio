#include "TDA_Codigos.h"
TDA_Codigos::TDA_Codigos(int num){
	nro=num;
	this->unario=getCUnario(num);
	gamma=getCGamma();
	
}
std::string TDA_Codigos::getCGamma(){
	std::string aux2;
	unsigned int loga=(int)(log(this->nro)/log(2));
	int unary= 1+loga;
	int bin=this->nro-(int)pow(2,loga);
	binario(bin,&aux2);
    for(int i=0;aux2.size()<loga;i++)
		aux2="0"+aux2;
	return (getCUnario(unary)+aux2);
}
std::string TDA_Codigos::getCUnario(int number){
	std::string aux;
	aux="0";
	for(int i=0;i<number-1;i++)
		aux="1"+aux;
	return aux;
}
void TDA_Codigos::binario(int num,std::string *bina){
	if (num == 0){
	  *bina="0";
  }else if (num == 1){
	  *bina="1";
  }else{
 	  binario(num/2,bina);
	  char* aux2=(char*)malloc(sizeof(char));
      std::string aux(itoa(num%2,aux2,10));
      *bina+=aux;
  }
}
