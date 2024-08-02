#pragma once
#include <gtkmm/window.h>
#include "virtual-keyboard-unstable-v1.h"
#include "input-method-unstable-v2.h"
#include "config.hpp"

class sysboard : public Gtk::Window {
	public:
		sysboard(const config_board &cfg);
		config_board config_main;

		void create_virtual_keyboard();
		void create_input_manager();
		void press_key(const int &keycode, const int &state);
		void set_modifier(const int &mod);

		zwp_virtual_keyboard_manager_v1 *keyboard_manager;
		zwp_virtual_keyboard_v1 *virtual_keyboard;

		zwp_input_method_manager_v2 *input_method_manager;
		zwp_input_method_v2 *input_method;

	private:
		GdkDisplay *gdk_display;
		GdkSeat *gdk_seat;
		wl_seat *seat;

		void initialize_protos();
		void load_layout();
};

extern "C" {
	sysboard *sysboard_create(const config_board &cfg);
}

