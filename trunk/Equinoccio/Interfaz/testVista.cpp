
#include "Interfaz.h"

int main(int argc, char** argv) {
	Gtk::Main kit(0, 0);
	Interfaz vista;

	vista.iniciar();

	Equinoccio::destruir();

	return 0;
}
