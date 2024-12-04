#pragma once
#include <gtkmm/box.h>
#include <gtkmm/gestureclick.h>

class sysboard;
class key;

class layout : public Gtk::Box {
	public:
		layout(sysboard* win, const std::string&, const int&);

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
		void handle_keycode(key*, const bool&);
};
