#include "layout.hpp"

#include <gtkmm/gestureclick.h>
#include <gtkmm/label.h>

layout::layout(const sysboard &window, std::vector<std::vector<std::string>> keymap) : Gtk::Box(Gtk::Orientation::VERTICAL) {
	set_halign(Gtk::Align::CENTER);
	for (ulong i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);

		for (ulong j = 0; j < keymap[i].size(); ++j) {
			std::string keymap_data = keymap[i][j];
			std::istringstream iss(keymap_data);
			std::string text;
			std::string keycode;
			std::string str_multiplier;
			iss >> text >> keycode >> str_multiplier;
			int code = std::stoi(keycode);
			int multiplier = std::stoi(str_multiplier);

			Gtk::Label *label = Gtk::make_managed<Gtk::Label>(text);

			if (j == 0) {
				label->set_size_request(btn_size * multiplier, btn_size);
				label->set_halign(Gtk::Align::START);
			}
			else if (j == keymap[i].size() - 1) {
				label->set_size_request(btn_size * multiplier, btn_size);
				label->set_halign(Gtk::Align::END);
			}
			else
				label->set_size_request(btn_size * multiplier, btn_size);

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
