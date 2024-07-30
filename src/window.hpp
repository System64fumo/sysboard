#pragma once
#include <gtkmm/window.h>
#include "virtual-keyboard-unstable-v1.h"

class sysboard : public Gtk::Window {
	public:
		sysboard();
		void create_virtual_keyboard();

		zwp_virtual_keyboard_manager_v1 *keyboard_manager;
		zwp_virtual_keyboard_v1 *virtual_keyboard;
};

extern "C" {
	sysboard *sysboard_create();
}

