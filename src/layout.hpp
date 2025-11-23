#pragma once
#include <gtkmm/grid.h>
#include <gtkmm/gestureclick.h>
#include <time.h>

class sysboard;
class key;

class layout : public Gtk::Grid {
	public:
		layout(sysboard* win, const std::string&);
		void handle_keycode(key*, const bool&);

	private:
		sysboard *window;
		std::string keymap_name;

		std::map<std::string, std::vector<std::pair<int, std::vector<std::string>>>> layout_map;
		std::vector<std::pair<int, std::vector<std::string>>> keymap;
		std::map<int, int> mod_map;
		int mods;
		long last_shift_time;
		bool shift_held;
		bool shift_temp;

		void load();
		long get_time_in_us();
};
