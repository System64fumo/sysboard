#include "layout.hpp"

#include <gtkmm/button.h>

layout::layout(std::vector<std::vector<std::string>> keymap) : Gtk::Box(Gtk::Orientation::VERTICAL) {
	for (long unsigned int i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
		box.set_halign(Gtk::Align::CENTER);

		for (long unsigned int j = 0; j < keymap[i].size(); ++j) {
			Gtk::Button button = Gtk::Button(keymap[i][j]);
			button.set_size_request(btn_size, btn_size);
			button.set_focusable(false);
			box.append(button);
		}

		append(box);
	}
}
