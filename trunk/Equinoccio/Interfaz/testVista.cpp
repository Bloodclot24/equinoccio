/*
 * testVista.cpp
 *
 *  Created on: 29/11/2009
 *      Author: luuu
 */

#include "Interfaz.h"

int main(int argc, char** argv) {
	Gtk::Main kit(0, 0);
	Interfaz vista;

	//agregar catalogos
	vista.agregarCatalogo("Musica");
	vista.agregarCatalogo("Audio");

	vista.run();

	return 0;
}
