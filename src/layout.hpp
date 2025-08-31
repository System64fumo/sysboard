#pragma once
#include <gtkmm/box.h>
#include <gtkmm/gestureclick.h>
#include <time.h>

class sysboard;
class key;

class layout : public Gtk::Box {
	public:
		layout(sysboard* win, const std::string&, const int&);
		void handle_keycode(key*, const bool&);

	private:
		sysboard *window;
		std::string keymap_name;
		int max_width;

		std::map<std::string, std::vector<std::vector<std::string>>> layout_map;
		std::vector<std::vector<std::string>> keymap;
		int btn_size;
		std::map<int, int> mod_map;
		int mods;
		long last_shift_time;
		bool shift_held;
		bool shift_temp;

		void load();
		long get_time_in_us();
};
