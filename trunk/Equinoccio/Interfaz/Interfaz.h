#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include "../Busquedas/Busqueda.h"
#include "../Thread/Thread.h"
#include "../Thread/Mutex.h"
#define ABRIR "xdg-open "

class Interfaz: public Thread {
private:
	Gtk::Window *main_window;	//ventana principal de la aplicacion
	Gtk::AboutDialog *about_window;	//ventana "acerca de"
	Glib::RefPtr<Gtk::Builder> builder;	//obtiene datos del archivo de la vista
	Gtk::FileChooserDialog *select_window; 	//ventana de seleccion de directorio
	Glib::RefPtr<Gtk::TreeSelection> selection;	//fila seleccionada

	Gtk::ProgressBar *progress_bar;	//barra que muestra actividad
	sigc::connection id_activity;	//id del refresh de la barra
	Gtk::Button *button_buscar;
	Gtk::Entry  *entry_consulta;
	Gtk::Statusbar *status_bar;

	bool activo;
	bool error;
	Glib::ustring catalogo; //catalogo en el que se esta buscando
	Glib::ustring consulta;
	sigc::connection id_esperando;

	void cargarMenu();
	void on_double_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_menu_about();
	void on_menu_help();
	void on_menu_quit();
	void on_button_add_clicked();
	void on_button_buscar_clicked();
	bool mover();
	void mostrarProgreso(Glib::ustring texto);
	void detenerBarra();
	void agregarFila(const std::string path);

	//Catalogo
	//Child widgets:
	Gtk::ComboBox *combo_catalogo;
	Glib::RefPtr<Gtk::ListStore> liststore_catalogo;
	class ColumnaCatalogo: public Gtk::TreeModel::ColumnRecord { 	//modelo de columnas
	public:
		ColumnaCatalogo() {
			add(m_col_catalogo);
			add(m_col_codigo);
		}

	    Gtk::TreeModelColumn<Glib::ustring> m_col_catalogo;
	    Gtk::TreeModelColumn<Glib::ustring> m_col_codigo;
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

	std::list<std::string> *paths_resultado;//mutex???
	bool esperarResultado();
	void finEspera();

//test
	Busqueda buscador;
	bool fin;
	Mutex mx_fin;

	void* run() {
		std::string cat = catalogo;
		std::string cons = consulta;
		paths_resultado = buscador.buscar(cons,cat);
		mx_fin.lock();
		fin = true;
		mx_fin.unlock();
		return NULL;
	}
public:
	Interfaz();
	~Interfaz();
	void iniciar();
	void agregarCatalogo(const std::string& catalogo,const std::string codigo);
	void setResultado(std::list<std::string> *paths) {
		paths_resultado = paths;
	}
};

#endif /* INTERFAZ_H_ */
