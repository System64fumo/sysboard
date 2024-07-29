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
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("funky.sys64.sysboard");
	app->hold();

	load_libsysboard();
	sysboard *window = sysboard_create_ptr();

	return app->run();
}
