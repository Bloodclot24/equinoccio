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
		builder->get_widget("combobox", combo);
		builder->get_widget("entry", entry_consulta);
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
	if (select_window->run() == Gtk::RESPONSE_OK);
		std::cout<<select_window->get_filename()<<std::endl;
	select_window->hide();
}

void Interfaz::on_button_buscar_clicked() {
	if(!activo){
		mostrarProgreso("Buscando..");
		std::cout<<"Consulta: "<<entry_consulta->get_text()<<std::endl;//" - Catalogo:"<<combo.get_title()<<std::endl;
		activo = true;
	}
	else {
		detenerBarra(); //por ahora :P
		activo = false;
	}
}

void Interfaz::mostrarProgreso(Glib::ustring texto){
	progress_bar->set_text(texto);
	//actualiza la barra de progreso cada un determinado tiempo
	id_activity = Glib::signal_timeout().connect(sigc::mem_fun(*this,
		        &Interfaz::mover), 200 );
}

bool Interfaz::mover() {
	progress_bar->pulse(); //mueve la barra de progreso
	return true;
}

void Interfaz::detenerBarra() {
	id_activity.disconnect();
	progress_bar->set_text(" ");
	progress_bar->set_fraction(0);
}

void Interfaz::run() {
	if (!error) {
		Gtk::Main* kit = Gtk::Main::instance();
		kit->run(*main_window);
	}
}
