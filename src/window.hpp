#pragma once
#include <gtkmm/window.h>
#include "virtual-keyboard-unstable-v1.h"
#include "input-method-unstable-v2.h"

class sysboard : public Gtk::Window {
	public:
		sysboard();
		void create_virtual_keyboard();
		void create_input_manager();
		void press_key(const int &keycode, const int &state);
		void set_modifier(const int &mod);

		zwp_virtual_keyboard_manager_v1 *keyboard_manager;
		zwp_virtual_keyboard_v1 *virtual_keyboard;

		zwp_input_method_manager_v2 *input_method_manager;
		zwp_input_method_v2 *input_method;

		int max_width = 0;
		int margin = 10;

	private:
		GdkDisplay *gdk_display;
		GdkSeat *gdk_seat;
		wl_seat *seat;

		void initialize_protos();
};

extern "C" {
	sysboard *sysboard_create();
}

