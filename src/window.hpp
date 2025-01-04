#pragma once
#include <map>
#include <gtkmm/window.h>
#include "virtual-keyboard-unstable-v1.h"
#include "input-method-unstable-v2.h"

class sysboard : public Gtk::Window {
	public:
		sysboard(const std::map<std::string, std::map<std::string, std::string>>&);
		std::map<std::string, std::map<std::string, std::string>> config_main;
		bool manual_mode = false;

		zwp_virtual_keyboard_manager_v1* keyboard_manager;
		zwp_input_method_manager_v2* input_method_manager;

		void create_virtual_keyboard();
		void create_input_manager();
		void press_key(const int&, const int&);
		void set_modifier(const int&);
		void load_layout();
		void handle_signal(const int&, const bool& manual = false);

	private:
		sigc::connection timeout_connection;
		GdkDisplay *gdk_display;
		GdkSeat *gdk_seat;
		wl_seat *seat;

		zwp_virtual_keyboard_v1* virtual_keyboard;
		zwp_input_method_v2* input_method;

		void initialize_protos();
};

extern "C" {
	sysboard *sysboard_create(const std::map<std::string, std::map<std::string, std::string>>&);
	void sysboard_signal(sysboard*, int);
}

