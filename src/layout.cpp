#include "layout.hpp"

#include <gtkmm/gestureclick.h>
#include <gtkmm/label.h>
#include <iostream>

layout::layout(const sysboard &window, std::vector<std::vector<std::string>> keymap) : Gtk::Box(Gtk::Orientation::VERTICAL) {
	for (long unsigned int i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
		box.set_halign(Gtk::Align::CENTER);

		for (long unsigned int j = 0; j < keymap[i].size(); ++j) {
			std::string keymap_data = keymap[i][j];
			std::istringstream iss(keymap_data);
			std::string text;
			std::string keycode;
			iss >> text >> keycode;
			int code = std::stoi(keycode);

			Gtk::Label *label = Gtk::make_managed<Gtk::Label>(text);
			label->set_size_request(btn_size, btn_size);
			label->set_focusable(false);
			auto gesture_click = Gtk::GestureClick::create();
			label->add_controller(gesture_click);

			// Handle events
			gesture_click->signal_pressed().connect([&, code](int, double, double) {
				window.press_key(code, 1);
			});
			gesture_click->signal_released().connect([&, code](int, double, double) {
				window.press_key(code, 0);
			});

			box.append(*label);
		}

		append(box);
	}
}
