#pragma once
#include "window.hpp"
#include "key.hpp"

#include <gtkmm/box.h>
#include <gtkmm/gestureclick.h>

class layout : public Gtk::Box {
	public:
		layout(sysboard *win, std::vector<std::vector<std::string>> keymap, const int &max_width);

	private:
		sysboard *window;
		double btn_size = 0;
		int mods = 0;

		void handle_keycode(key *kbd_key, const bool &pressed);
};
