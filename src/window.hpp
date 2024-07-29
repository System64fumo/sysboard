#pragma once
#include <gtkmm/window.h>

class sysboard : public Gtk::Window {
	public:
		sysboard();
};

extern "C" {
	sysboard *sysboard_create();
}

