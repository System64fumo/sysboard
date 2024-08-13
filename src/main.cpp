#include "main.hpp"
#include "git_info.hpp"

#include <gtkmm/application.h>
#include <iostream>
#include <dlfcn.h>

void handle_signal(int signum) {
	sysboard_handle_signal_ptr(window, signum);
}

void load_libsysboard() {
	void* handle = dlopen("libsysboard.so", RTLD_LAZY);
	if (!handle) {
		std::cerr << "Cannot open library: " << dlerror() << '\n';
		exit(1);
	}

	sysboard_create_ptr = (sysboard_create_func)dlsym(handle, "sysboard_create");
	sysboard_handle_signal_ptr = (sysboard_handle_signal_func)dlsym(handle, "sysboard_signal");

	if (!sysboard_create_ptr || !sysboard_handle_signal_ptr) {
		std::cerr << "Cannot load symbols: " << dlerror() << '\n';
		dlclose(handle);
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	while (true) {
		switch(getopt(argc, argv, "m:dH:dl:dvh")) {
			case 'm':
				config_main.margin = std::stoi(optarg);
				continue;

			case 'H':
				config_main.height_multiplier = std::stod(optarg);
				continue;

			case 'l':
				config_main.layout = optarg;
				continue;

			case 'v':
				std::cout << "Commit: " << GIT_COMMIT_MESSAGE << std::endl;
				std::cout << "Date: " << GIT_COMMIT_DATE << std::endl;
				return 0;

			case 'h':
			default :
				std::cout << "usage:" << std::endl;
				std::cout << "  sysboard [argument...]:\n" << std::endl;
				std::cout << "arguments:" << std::endl;
				std::cout << "  -m	Set margin" << std::endl;
				std::cout << "  -H	Set height multiplier" << std::endl;
				std::cout << "  -l	Set layout" << std::endl;
				std::cout << "  -v	Prints version info" << std::endl;
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
	window = sysboard_create_ptr(config_main);
	(void)window; // This is to avoid the unused variable warning

	// Catch signals
	signal(SIGUSR1, handle_signal);
	signal(SIGUSR2, handle_signal);
	signal(SIGRTMIN, handle_signal);

	return app->run();
}
