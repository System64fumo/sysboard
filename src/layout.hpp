#pragma once
#include "window.hpp"
#include "key.hpp"

#include <gtkmm/box.h>
#include <gtkmm/gestureclick.h>

class layout : public Gtk::Box {
	public:
		layout(sysboard *win, const std::string &keymap_name, const int &max_width);

	private:
		sysboard *window;
		std::string keymap_name;
		int max_width;

		std::map<std::string, std::vector<std::vector<std::string>>> layout_map;
		std::vector<std::vector<std::string>> keymap;
		int btn_size = 0;
		std::map<int, int> mod_map;
		int mods = 0;

		void load();
		void handle_keycode(key *kbd_key, const bool &pressed);
};
