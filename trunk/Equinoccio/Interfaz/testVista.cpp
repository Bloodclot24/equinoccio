
#include "Interfaz.h"
int main(int argc, char** argv) {
	Gtk::Main kit(0, 0);
	Interfaz vista;

	//agregar catalogos
	vista.agregarCatalogo("Imagen", "IMG");
	vista.agregarCatalogo("Audio", "SND");
	vista.agregarCatalogo("Texto", "TXT");
	vista.agregarCatalogo("Fuentes", "SRC");

	vista.iniciar();

	return 0;
}
