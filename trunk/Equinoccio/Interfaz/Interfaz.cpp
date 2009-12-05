#include "Interfaz.h"

#define WINDOW_FILE "Interfaz/interfaz.glade"

Interfaz::Interfaz() {
	error=false;
	activo=false;
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

		builder->get_widget("combobox", combo_catalogo);
		liststore_catalogo= Gtk::ListStore::create(columna_catalogo);
		combo_catalogo->set_model(liststore_catalogo);
		builder->get_widget("entry", entry_consulta);
		builder->get_widget("statusbar", status_bar);

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

void Interfaz::on_button_add_clicked() {
	if (select_window->run() == Gtk::RESPONSE_OK) {
		std::cout<<select_window->get_filename()<<std::endl;//if !activo?????????????????
		mostrarProgreso("Indexando");
		Glib::ustring text = "Agregando directorio: ";
		text += select_window->get_filename();
		activo = true;
		status_bar->push(text);
	}
	select_window->hide();
}

void Interfaz::on_button_buscar_clicked() {
	if(!activo){
		Gtk::TreeModel::iterator iter_active= combo_catalogo->get_active();
		if(iter_active) {
			liststore_busqueda->clear();
			Gtk::TreeModel::Row row= *iter_active;
			catalogo= row[columna_catalogo.m_col_catalogo];
			Glib::ustring cod_cat = row[columna_catalogo.m_col_codigo];
			std::cout<<"Consulta: "<<entry_consulta->get_text()<<" - Catalogo: "<<catalogo<<" - Codigo: "<<cod_cat<<std::endl;
			mostrarProgreso("Buscando..");
			activo = true;
			Glib::ustring text = " ";
			status_bar->push(text);

			agregarFila(entry_consulta->get_text());//test

		} else {
			Glib::ustring text = "Debe ingresar un catalogo";
			status_bar->push(text);
		}
	}
	else {
		detenerBarra(); //por ahora :P
		activo = false;
		Glib::ustring text = " ";
		status_bar->push(text);
	}
}

void Interfaz::on_double_click(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* column) {
	Gtk::TreeModel::iterator iter = selection->get_selected();
	Gtk::TreeModel::Row row = *iter;
	std::cout<<"Doble click. Path: "<< row.get_value(columna_busqueda.m_col_path)<<std::endl;
	std::string comando = ABRIR;
	comando += row.get_value(columna_busqueda.m_col_path);
	system(comando.c_str()); //TODO hay que escapear los caracteres!
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
	progress_bar->set_text(" ");
	progress_bar->set_fraction(0);
	progress_bar->hide();
}

void Interfaz::run() {
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
	row[columna_busqueda.m_col_catalogo] = catalogo;
}

