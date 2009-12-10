#ifndef INTERFAZ_H_
#define INTERFAZ_H_

#include <gtkmm.h>
#include <glib.h>
#include <iostream>
#include "../Busquedas/Busqueda.h"
#include "../Thread/Thread.h"
#include "../Thread/Mutex.h"
#include "../Equinoccio.h"
#define ABRIR "xdg-open "

#define E_NADA 	0
#define E_INDEX 1
#define E_REM	2
#define E_RALL 	3
#define E_LIST	4
#define E_SEARCH 5

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
	Gtk::ToggleButton *check_box;

	Glib::ustring catalogo; //catalogo en el que se esta buscando
	Glib::ustring descr_catalogo;
	Glib::ustring consulta;
	Glib::ustring directorio;
	sigc::connection id_esperando;
	bool rankeada;

	void cargarMenu();
	void on_double_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column);
	void on_menu_about();
	void on_menu_help();
	void on_menu_quit();
	void on_button_add_clicked();
	void on_button_rem_clicked();
	void on_button_remall_clicked();
	void on_button_buscar_clicked();
	void on_button_list_clicked();
	bool mover();
	void mostrarProgreso(Glib::ustring texto);
	void detenerBarra();
	void agregarFila(const std::string path);
	bool esperarResultado();
	void finEspera();
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

	class ColumnaDirs: public Gtk::TreeModel::ColumnRecord {
	public:
		ColumnaDirs() {
			add(m_col_dir);
		}
		Gtk::TreeModelColumn<Glib::ustring> m_col_dir;
	};

	Glib::RefPtr<Gtk::ListStore> liststore_busqueda;
	Glib::RefPtr<Gtk::ListStore> liststore_dirs;
	ColumnaBusqueda columna_busqueda;
	ColumnaDirs columna_dir;
	Gtk::TreeView *tree_view;
	Gtk::TreeView *tree_dirs;

	Mutex mx_fin;
	bool fin;
	int estado;	//indica que se esta realizando. Si activo, es distinto de cero
	bool error;
	std::list<std::string> *paths_resultado;
	std::list<std::string> *lista_dirs;

	void* run() {
		if (estado == E_SEARCH) {
			//busqueda
			std::string cat = catalogo;
			std::string cons = consulta;
			const char* com_c[] = {"./Equinoccio", ARG_CAT, cat.c_str(), ARG_SEARCH, cons.c_str()};
			Busqueda::rankeada = rankeada;
			Equinoccio::silencio = true;
			Equinoccio::main(5, com_c);
			paths_resultado = Equinoccio::getPaths();
		}else {
		if (estado == E_INDEX) {
			//agrego directorio
			std::string dir = directorio;
			const char* com_c[] = {"./Equinoccio", ARG_ADD, dir.c_str()};
			Equinoccio::silencio = false;
			Equinoccio::main(3,com_c);
		}
		else {
		if (estado == E_LIST) {
			Equinoccio::silencio = true;
			lista_dirs = Equinoccio::getDirIndexados();
		}else{
		if (estado ==  E_RALL) {
			FileManager::borrarIndice();
		}else{
		if (estado == E_REM) {
			std::string dir = directorio;
			const char* com_c[] = {"./Equinoccio", ARG_DEL, dir.c_str()};
			Equinoccio::silencio = false;
			Equinoccio::main(3,com_c);
		}
		}}}}
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
