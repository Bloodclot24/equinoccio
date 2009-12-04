#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>

class Interfaz {
private:
	Gtk::Window *main_window; //ventana principal de la aplicacion
	Gtk::AboutDialog *about_window; //ventana "acerca de"
	Glib::RefPtr<Gtk::Builder> builder;//obtiene datos del archivo de la vista
	Gtk::FileChooserDialog *select_window; //ventana de seleccion de directorio

	Gtk::ProgressBar *progress_bar;//barra que muestra actividad
	sigc::connection id_activity;//id del refresh de la barra
	Gtk::Button *button_buscar;
	Gtk::Entry  *entry_consulta;
	Gtk::Statusbar *status_bar;

	bool activo;
	bool error;

	void cargarMenu();

	void on_menu_about();
	void on_menu_help();
	void on_menu_quit();
	void on_button_add_clicked();
	void on_button_buscar_clicked();
	bool mover();
	void mostrarProgreso(Glib::ustring texto);
	void detenerBarra();

	//Catalogo
	//Child widgets:
	Gtk::ComboBox *combo_catalogo;
	Glib::RefPtr<Gtk::ListStore> liststore_catalogo;
	class ColumnaCatalogo: public Gtk::TreeModel::ColumnRecord { 	//modelo de columnas
	public:
		ColumnaCatalogo() {
			add(m_col_catalogo);
		}

	    Gtk::TreeModelColumn<Glib::ustring> m_col_catalogo;
	};
	ColumnaCatalogo columna_catalogo;


	class ColumnaBusqueda: public Gtk::TreeModel::ColumnRecord {
	public:
		ColumnaBusqueda() {
			add(m_col_path);
			add(m_col_catalogo);
		}

	    Gtk::TreeModelColumn<Glib::ustring> m_col_path;
	    Gtk::TreeModelColumn<Glib::ustring> m_col_catalogo;
	};

	Glib::RefPtr<Gtk::ListStore> liststore_busqueda;
	ColumnaBusqueda columna_busqueda;
	Gtk::TreeView *tree_view;

public:
	Interfaz();
	~Interfaz();
	void run();
	void agregarCatalogo(const std::string& catalogo);
};

#endif /* INTERFAZ_H_ */
