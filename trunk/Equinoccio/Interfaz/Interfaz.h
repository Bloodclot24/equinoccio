#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>

class Interfaz {
private:
	Gtk::Window *main_window; //ventana principal de la aplicacion
	Glib::RefPtr<Gtk::Builder> builder;//obtiene datos del archivo de la vista
	bool error;

	void cargarMenu();

	void on_menu_about();
	void on_menu_help();
	void on_menu_quit();
	void on_button_add_clicked();

public:
	Interfaz();
	~Interfaz();
	void run();
};

#endif /* INTERFAZ_H_ */
