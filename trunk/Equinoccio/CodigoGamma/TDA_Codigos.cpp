#include "TDA_Codigos.h"

uint32_t TDA_Codigos::getNGamma(std::string &str){
	uint32_t una=1;/*Numero en Unario*/
	uint32_t bin=0;/*Numero en Binario*/
	uint32_t spbin=0;/*size parte binaria*/
	uint32_t sizestr = str.size();/*Size total*/
	uint32_t puna = str.find_first_of("0")+1;/*Donde termina la parte unaria*/
	std::string strb =str.substr(puna,sizestr);/*Obtengo el substring de la parte binaria*/
	if (puna-1 == std::string::npos) return -1; /* Si no tengo el 0, salgo con error */
	spbin=sizestr-puna;/*Calculo el size de la parte binaria*/
	   if (puna>1)/*Si la parte unaria es mayor que 1 sino vale 1*/
		   una=pow(2,puna-1);/*Calculo lo de la parte unaria*/
	   if(spbin < puna-1) return -1;/* Si no tengo la cantidad de bits necesaria, salgo con error*/
	   for(uint32_t i=0;i<spbin;i++)
		   bin+= (((short)strb.at(spbin-i-1))-48)*pow(2,(i)); /*Teorema F. Numeracion*/
	 return (bin+una);
}
std::string TDA_Codigos::getCGamma(uint32_t nro){
	std::string aux2;
	uint32_t loga=(uint32_t)(logl(nro)/logl(2.0));/*Calculo la funcion piso del logaritmo en base 2 de x */
	uint32_t unary= 1+loga; /*le sumo 1 a ese numero*/
	uint32_t bin=nro-(uint32_t)pow(2,loga); /*Calculo la parte Binaria*/
	if (nro > 1)/*Valido para no hacer cuentas inncesarias*/
	     binario(bin,aux2);/*Paso a Binario en Formato String*/
	while(aux2.size()<loga)/*Completo con 0 adelante para dar el formato de bin*/
	     aux2="0"+aux2;
	return (getCUnario(unary)+aux2);/*Devuelvo la Cadena*/
}
std::string TDA_Codigos::getCUnario(uint32_t number){/*Codifico el Unario de Number en formato string*/
	std::string aux;
	aux="0";
	for(uint32_t i=0;i<number-1;i++)
		aux="1"+aux;
	return aux;
}
void TDA_Codigos::binario(uint32_t num,std::string& bina){/*Paso a Binario con logica de desplazamiento*/
	do{
		if ((num % 2) != 0)
			bina.insert(0,"1");
		else bina.insert(0,"0");
	}while ((num = num >> 1) != 0);
}
