#include "Merge.h"
#include <stdio.h>

// template <class t>
// int Merger<t>::Merge(const std::vector<std::string>& nombreParticiones, 
// 		      const std::string& nombreSalida){
	  
//   // if(nombreParticiones.size() < 2)
//   //   return -1;

//   std::ifstream* archivo;
//   std::ofstream salida(nombreSalida.c_str());
//   unsigned menor=0;
//   std::vector<std::ifstream*> particiones;
//   std::vector<t*> registro;

//   /* leo el primer archivo de cada particion, si pudo ser
//    * abierto, lo agrego al vector de particiones */
//   for(unsigned i=0;i<nombreParticiones.size();i++){
//     archivo = new std::ifstream(nombreParticiones[i].c_str());
//     if(archivo->is_open()){
//       particiones.push_back(archivo);
//       registro.push_back(NULL);
//     }else
//         /* Si el archivo no pudo ser abierto lo libero */
// 	delete archivo;
//   }

//   /* Si la cantidad de archivos agregados es mayor a 1 */
//   if(particiones.size() > 0){
//     /* lectura inicial de registros de cada particion */
//     for(unsigned i=0;i<particiones.size();i++){
//       registro[i] = t::leer(*particiones[i], 0);
//       /* Si el registro es NULL, borro la particion */
//       if(registro[i] == NULL){
//         delete particiones[i];
//         /* disminuyo el tamaño de las particiones */
//         if(i < particiones.size()-1){
//           particiones[i] = particiones[particiones.size()-1];
//           registro[i] = registro[registro.size()-1];
//           i--; // solo bajo supervision de un adulto
//         }
// 		particiones.pop_back();
// 		registro.pop_back();

// 	/* Si el registro leido es mas chico al menor, modifico
// 	 * el menor */
//       } else if(*registro[i] < *registro[menor]){
//         menor = i;

//         /* Si los registros son iguales los uno */
//       } else if((i!=menor) && !(*registro[i] > *registro[menor])){
//         registro[menor]->unir(*registro[i]);
//         delete registro[i];
//         i--; //idem
//       }
//     }

//     /* Mientras haya particiones */
//     while(particiones.size() > 0){
//       /* escribo el menor en el archivo de salida*/
//       registro[menor]->escribir(salida, 0);
//       delete registro[menor];
//       /*leo el siguiente*/
//       registro[menor] = t::leer(*particiones[menor],0);
//       if(registro[menor] == NULL){
//         delete particiones[menor];
//         /* guardo la ultima particion y el ultimo registro
//          * en la posicion del menor que escribi*/
//         if(menor < particiones.size()-1){
//           particiones[menor] = particiones[particiones.size()-1];
//           registro[menor] = registro[registro.size()-1];
// 		}
//         particiones.pop_back();
//         registro.pop_back();
//       }
//       menor = 0;
//       /*Para todo el resto de las particiones*/
//       for(unsigned i=0;i<particiones.size();i++){
//         /*Busco el menor*/
//         if(*registro[i] < *registro[menor]){
//           menor = i;
//         } else if((i!=menor) && !(*registro[i] > *registro[menor])){
//           registro[menor]->unir(*registro[i]);
//           delete registro[i];
//           registro[i] = t::leer(*particiones[i],0);
//           if(registro[i] == NULL){
//             delete particiones[i];
//             /* guardo la ultima particion y el ultimo registro
//          	* en la posicion del menor que escribi*/
//             if(i < particiones.size()-1){
//               particiones[i] = particiones[particiones.size()-1];
//               registro[i] = registro[registro.size()-1];
//             }
//             particiones.pop_back();
//             registro.pop_back();
//             i--;
//           }
//         }
//       }
//     }
//   }

//   /* libero las particiones */
//   for(unsigned i=0;i<particiones.size();i++)
//     delete particiones[i];

//   /* elimino las particiones */
//   for(unsigned i=0;i<nombreParticiones.size();i++)
//        remove(nombreParticiones[i].c_str());

//   return 0;
// }
