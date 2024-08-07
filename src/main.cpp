#include "main.hpp"

#include <gtkmm/application.h>
#include <iostream>
#include <dlfcn.h>

void load_libsysboard() {
	void* handle = dlopen("libsysboard.so", RTLD_LAZY);
	if (!handle) {
		std::cerr << "Cannot open library: " << dlerror() << '\n';
		exit(1);
	}

	sysboard_create_ptr = (sysboard_create_func)dlsym(handle, "sysboard_create");

	if (!sysboard_create_ptr) {
		std::cerr << "Cannot load symbols: " << dlerror() << '\n';
		dlclose(handle);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	while (true) {
		switch(getopt(argc, argv, "m:dH:dl:dh")) {
			case 'm':
				config_main.margin = std::stoi(optarg);
				continue;

			case 'H':
				config_main.height_multiplier = std::stod(optarg);
				continue;

			case 'l':
				config_main.layout = optarg;
				continue;

			case 'h':
			default :
				std::cout << "usage:" << std::endl;
				std::cout << "  sysboard [argument...]:\n" << std::endl;
				std::cout << "arguments:" << std::endl;
				std::cout << "  -m	Set margin" << std::endl;
				std::cout << "  -H	Set height multiplier" << std::endl;
				std::cout << "  -l	Set layout" << std::endl;
				std::cout << "  -h	Show this help message" << std::endl;
				return 0;

			case -1:
				break;
			}

			break;
	}

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("funky.sys64.sysboard");
	app->hold();

	load_libsysboard();
	sysboard *window = sysboard_create_ptr(config_main);
	(void)window; // This is to avoid the unused variable warning

	return app->run();
}
