#include "Interfaz.h"
#include "../Util/Util.h"

#define WINDOW_FILE "Interfaz/interfaz.glade"

Interfaz::Interfaz() {
	error=false;
	estado = E_NADA;
	paths_resultado = NULL;
	fin = false;
	consulta = "";
	directorio = "";
	try {
		builder = Gtk::Builder::create_from_file(WINDOW_FILE);
		main_window = 0;
		builder->get_widget("window", main_window);//obtengo la ventana principal
		about_window = 0;
		builder->get_widget("about", about_window);
		select_window = 0;
		builder->get_widget("filechooserdialog", select_window);
		select_window->add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
		select_window->add_button(Gtk::Stock::APPLY, Gtk::RESPONSE_OK);
		builder->get_widget("button_search", button_buscar);
		button_buscar->signal_clicked().connect(sigc::mem_fun(*this,
					&Interfaz::on_button_buscar_clicked));
		builder->get_widget("progressbar", progress_bar);
		progress_bar->hide();
		builder->get_widget("checkbutton", check_box);

		builder->get_widget("combobox", combo_catalogo);
		liststore_catalogo= Gtk::ListStore::create(columna_catalogo);
		combo_catalogo->set_model(liststore_catalogo);
		builder->get_widget("entry", entry_consulta);
		entry_consulta->signal_activate().connect(sigc::mem_fun(*this,
					&Interfaz::on_button_buscar_clicked));
		builder->get_widget("statusbar", status_bar);

		builder->get_widget("treeview2", tree_dirs);
		liststore_dirs= Gtk::ListStore::create(columna_dir);
		tree_dirs->set_model(liststore_dirs);
		tree_dirs->append_column("Directorios Indexados", columna_dir.m_col_dir);


		builder->get_widget("treeview", tree_view);
		liststore_busqueda= Gtk::ListStore::create(columna_busqueda);
		tree_view->set_model(liststore_busqueda);
		tree_view->append_column("Catalogo", columna_busqueda.m_col_catalogo);
		tree_view->append_column("Archivo", columna_busqueda.m_col_path);
		selection = tree_view->get_selection();
		tree_view->signal_row_activated().connect(sigc::mem_fun(*this,
									&Interfaz::on_double_click));
		cargarMenu();

	} catch (Glib::FileError& ex1) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex1.what() << std::endl;
		error = true;
	} catch (Glib::MarkupError& ex2) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex2.what() << std::endl;
		error = true;
	} catch (Gtk::BuilderError& ex3) {
		std::cerr << "error al cargar el archivo de la vista" << std::endl;
		std::cerr << ex3.what() << std::endl;
		error = true;
	}
	//agregar catalogos
	agregarCatalogo("Imagen", "IMG");
	agregarCatalogo("Audio", "SND");
	agregarCatalogo("Texto", "TXT");
	agregarCatalogo("Fuentes", "SRC");

}

Interfaz::~Interfaz() {}

void Interfaz::cargarMenu() {
	/* menu */
	Glib::RefPtr<Gtk::ActionGroup> menu_archivo;
	Glib::RefPtr<Gtk::ActionGroup> menu_ayuda;
	Glib::RefPtr<Gtk::UIManager> menu_UIManager;
	Gtk::Widget* menu;

	menu_archivo = Gtk::ActionGroup::create();
	menu_ayuda = Gtk::ActionGroup::create();

	menu_archivo->add(Gtk::Action::create("FileMenu", "Archivo"));
	menu_archivo->add(Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
			sigc::mem_fun(*this, &Interfaz::on_menu_quit));
	menu_archivo->add(Gtk::Action::create("FileAdd", Gtk::Stock::OPEN,
			"_Agregar Directorio", "Indexa el nuevo directorio"),
			sigc::mem_fun(*this, &Interfaz::on_button_add_clicked));
	menu_archivo->add(Gtk::Action::create("DirRem", Gtk::Stock::REMOVE,
			"_Eliminar Directorio", "Saca al directorio del indice de busqueda"),
			sigc::mem_fun(*this, &Interfaz::on_button_rem_clicked));
	menu_archivo->add(Gtk::Action::create("IdxRem", Gtk::Stock::DELETE,
			"_Eliminar Todos", "Saca del indice a todos los directorios"),
			sigc::mem_fun(*this, &Interfaz::on_button_remall_clicked));
	menu_archivo->add(Gtk::Action::create("List", Gtk::Stock::DIRECTORY,
				"_Listar Directorios", "Lista todos los directorios"),
				sigc::mem_fun(*this, &Interfaz::on_button_list_clicked));
	menu_ayuda->add(Gtk::Action::create("HelpMenu", "Ayuda"));
	menu_ayuda->add(Gtk::Action::create("HelpAbout", Gtk::Stock::ABOUT,"A_cerca De", "Acerca de Equinoccio"),
			sigc::mem_fun(*this, &Interfaz::on_menu_about));
	menu_ayuda->add(Gtk::Action::create("HelpMan", Gtk::Stock::HELP,"_Manual", "Manual de usuario"),
			sigc::mem_fun(*this, &Interfaz::on_menu_help));

	menu_UIManager = Gtk::UIManager::create();
	menu_UIManager->insert_action_group(menu_archivo);
	menu_UIManager->insert_action_group(menu_ayuda);

	main_window->add_accel_group(menu_UIManager->get_accel_group());

	//layout
	Glib::ustring ui_info = "<ui>"
		"  <menubar name='MenuBar'>"
		"    <menu action='FileMenu'>"
		"      <menuitem action='FileAdd'/>"
		"      <menuitem action='DirRem'/>"
		"      <separator/>"
		"      <menuitem action='IdxRem'/>"
		"      <separator/>"
		"      <menuitem action='List'/>"
		"      <separator/>"
		"      <menuitem action='FileQuit'/>"
		"    </menu>"
		"    <menu action='HelpMenu'>"
		"      <menuitem action='HelpMan'/>"
		"      <menuitem action='HelpAbout'/>"
		"    </menu>"
		"  </menubar>"
		"</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{
		menu_UIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " << ex.what();
	}
#else
	std::auto_ptr<Glib::Error> ex;
	menu_UIManager->add_ui_from_string(ui_info, ex);
	if (ex.get()) {
		std::cerr << "building menus failed: " << ex->what();
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
	menu = menu_UIManager->get_widget("/MenuBar");
	Gtk::Box *box;
	builder->get_widget("menubox", box);
	box->pack_end(*menu, Gtk::PACK_SHRINK);
}

void Interfaz::on_menu_about() {
	about_window->run();
	about_window->hide();
}

void Interfaz::on_menu_help() {}

void Interfaz::on_menu_quit() {
	main_window->hide();
}

void Interfaz::on_button_list_clicked() {
	if (estado == E_NADA) {
		estado = E_LIST;
		mostrarProgreso("Listando");
		status_bar->push("Listando los Directorios Indexados");
		button_buscar->set_sensitive(false);
		entry_consulta->set_sensitive(false);
		id_esperando = Glib::signal_timeout().connect(sigc::mem_fun(*this,
								&Interfaz::esperarResultado), 200 );
		this->execute();
	}
}

void Interfaz::on_button_remall_clicked() {
	if (estado == E_NADA) {
		estado = E_RALL;
		mostrarProgreso("Eliminando");
		status_bar->push("Eliminando Todos los Directorios del Indice");
		button_buscar->set_sensitive(false);
		entry_consulta->set_sensitive(false);
		id_esperando = Glib::signal_timeout().connect(sigc::mem_fun(*this,
								&Interfaz::esperarResultado), 200 );
		this->execute();
	}
}

void Interfaz::on_button_rem_clicked() {
	if (estado == E_NADA) {
		if (select_window->run() == Gtk::RESPONSE_OK) {
			estado = E_REM;
			directorio = select_window->get_filename();
			mostrarProgreso("Eliminando");
			Glib::ustring text = "Eliminando directorio: ";
			text += directorio;
			text += " del indice.";
			status_bar->push(text);
			button_buscar->set_sensitive(false);
			entry_consulta->set_sensitive(false);
			id_esperando = Glib::signal_timeout().connect(sigc::mem_fun(*this,
									&Interfaz::esperarResultado), 200 );
			this->execute();
		}
		select_window->hide();
	}
}

void Interfaz::on_button_add_clicked() {
	if (estado == E_NADA) {
		if (select_window->run() == Gtk::RESPONSE_OK) {
			estado = E_INDEX;
			directorio = select_window->get_filename();
			mostrarProgreso("Indexando");
			Glib::ustring text = "Agregando directorio: ";
			text += directorio;
			status_bar->push(text);
			button_buscar->set_sensitive(false);
			entry_consulta->set_sensitive(false);
			id_esperando = Glib::signal_timeout().connect(sigc::mem_fun(*this,
									&Interfaz::esperarResultado), 200 );
			this->execute();
		}
		select_window->hide();
	}
}

void Interfaz::on_button_buscar_clicked() {
	if(estado == E_NADA){
		Gtk::TreeModel::iterator iter_active= combo_catalogo->get_active();
		if(iter_active) {
			consulta = entry_consulta->get_text();
			if (consulta != "") {
				estado = E_SEARCH;
				liststore_busqueda->clear();
				Gtk::TreeModel::Row row= *iter_active;
				catalogo= row[columna_catalogo.m_col_codigo];
				descr_catalogo = row[columna_catalogo.m_col_catalogo];
				Glib::ustring cod_cat = row[columna_catalogo.m_col_codigo];
				std::cout<<"Consulta: "<<consulta<<" - Catalogo: "<<descr_catalogo<<" - Codigo: "<<cod_cat<<std::endl;
				mostrarProgreso("Buscando..");
				status_bar->push(" ");
				button_buscar->set_sensitive(false);
				entry_consulta->set_sensitive(false);
				paths_resultado = NULL;
				rankeada = check_box->get_active();
				id_esperando = Glib::signal_timeout().connect(sigc::mem_fun(*this,
							&Interfaz::esperarResultado), 200 );
				this->execute();
			}
			else {
				status_bar->push("Debe ingresar consulta");
			}
		} else {
			status_bar->push("Debe ingresar un catalogo");
		}
	}
}

void Interfaz::on_double_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	Gtk::TreeModel::Row row = *iter;
	std::cout<<"Doble click. Path: "<< row.get_value(columna_busqueda.m_col_path)<<std::endl;
	std::string comando = ABRIR;
	comando += '"';
	comando += row.get_value(columna_busqueda.m_col_path);
	comando += '"';
	system(comando.c_str());
}

void Interfaz::mostrarProgreso(Glib::ustring texto){
	progress_bar->set_text(texto);
	//actualiza la barra de progreso cada un determinado tiempo
	id_activity = Glib::signal_timeout().connect(sigc::mem_fun(*this,
		        &Interfaz::mover), 200 );
	progress_bar->show();
}

bool Interfaz::mover() {
	progress_bar->pulse(); //mueve la barra de progreso
	return true;
}

void Interfaz::detenerBarra() {
	id_activity.disconnect();
	estado = E_NADA;
	progress_bar->set_text(" ");
	progress_bar->set_fraction(0);
	progress_bar->hide();
	button_buscar->set_sensitive(true);
	entry_consulta->set_sensitive(true);
}

void Interfaz::iniciar() {
	if (!error) {
		Gtk::Main* kit = Gtk::Main::instance();
		kit->run(*main_window);
		//liberar
	}
}

void Interfaz::agregarCatalogo(const std::string& catalogo,const std::string codigo) {
	Gtk::TreeModel::Row row = *(liststore_catalogo->append());
	row[columna_catalogo.m_col_catalogo] = catalogo;
	row[columna_catalogo.m_col_codigo] = codigo;
}

void Interfaz::agregarFila(const std::string path) {
	Gtk::TreeModel::Row row = *(liststore_busqueda->append());
	row[columna_busqueda.m_col_path] = path;
	row[columna_busqueda.m_col_catalogo] = descr_catalogo;
}

bool Interfaz::esperarResultado() {
	mx_fin.lock();
	if (fin) {
		finEspera();
	}
	mx_fin.unlock();
	return true;
}

void Interfaz::finEspera() {
	id_esperando.disconnect();
	if(estado==E_INDEX) {
		Glib::ustring text = "Directorio ";
		text += directorio;
		text += " indexado";
		directorio = "";
		status_bar->push(text);
	}else {
	if (estado==E_SEARCH) {
		if (paths_resultado != NULL) {
			Glib::ustring res= "Se encontraron ";
			res += Util::intToString(paths_resultado->size());
			res += " resultados.";
			status_bar->push(res);
			std::list<std::string>::iterator it;
			for(it=paths_resultado->begin(); it!=paths_resultado->end(); it++) {
				agregarFila(*it);
			}
			delete paths_resultado;
			paths_resultado = NULL;
		}
		else
			status_bar->push("No se encontraron resultados.");
	}else {
	if (estado==E_LIST) {
		liststore_dirs->clear();
		if (!lista_dirs->empty()) {
			std::list<std::string>::iterator it;
			for (it=lista_dirs->begin(); it != lista_dirs->end(); it++) {
				Gtk::TreeModel::Row row = *(liststore_dirs->append());
				row[columna_dir.m_col_dir] = *it;
			}
			Glib::ustring text = Util::intToString(lista_dirs->size());
			text += " directorios indexados.";
			status_bar->push(text);
		}
		else
			status_bar->push("No hay directorios indexados.");
	}else{
	if (estado==E_RALL){
		status_bar->push("Indice eliminado.");
		liststore_dirs->clear();
	}

//rellenar
	}}}
	detenerBarra();
	fin = false;
}
