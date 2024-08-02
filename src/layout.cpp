#include "layout.hpp"

#include <algorithm>

layout::layout(sysboard *win, std::vector<std::vector<std::string>> keymap) : Gtk::Box(Gtk::Orientation::VERTICAL) {
	window = win;
	set_halign(Gtk::Align::CENTER);

	// TODO: Clean this up
	// Dynamic scaling
	auto largest_vec_it = std::max_element(
		keymap.begin(), keymap.end(),
		[](const std::vector<std::string>& a, const std::vector<std::string>& b) {
			return a.size() < b.size();
		}
	);

	btn_size = win->max_width / largest_vec_it->size();

	double pixels = 0;

	for (const std::string& str : *largest_vec_it) {
		std::istringstream iss(str);
		std::string text;
		std::string keycode;
		std::string str_multiplier;
		iss >> text >> keycode >> str_multiplier;
		int code = std::stoi(keycode);
		double multiplier = std::stod(str_multiplier);

		pixels += btn_size * multiplier;
	}

	double scaling_factor = win->max_width / pixels;
	btn_size = btn_size * scaling_factor;

	// Populate layout
	for (ulong i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
		box.set_halign(Gtk::Align::CENTER);

		for (ulong j = 0; j < keymap[i].size(); ++j) {
			std::string keymap_data = keymap[i][j];
			std::istringstream iss(keymap_data);
			std::string text;
			std::string keycode;
			std::string str_multiplier;
			iss >> text >> keycode >> str_multiplier;
			int code = std::stoi(keycode);
			double multiplier = std::stod(str_multiplier);

			Gtk::Label *label = Gtk::make_managed<Gtk::Label>(text);

			if (j == 0) {
				label->set_size_request(btn_size * multiplier, btn_size * height_multiplier);
				label->set_halign(Gtk::Align::START);
			}
			else if (j == keymap[i].size() - 1) {
				label->set_size_request(btn_size * multiplier, btn_size * height_multiplier);
				label->set_halign(Gtk::Align::END);
			}
			else
				label->set_size_request(btn_size * multiplier, btn_size * height_multiplier);

			label->set_focusable(false);
			Glib::RefPtr<Gtk::GestureClick> gesture_click = Gtk::GestureClick::create();
			label->add_controller(gesture_click);

			// Handle events
			// TODO: Handle special events (code 0) based on their label
			// TODO: Toggle mod keys
			gesture_click->signal_pressed().connect([&, label, code](int, double, double) {
				handle_keycode(label, code, true);
			});
			gesture_click->signal_released().connect([&, label, code](int, double, double) {
				handle_keycode(label, code, false);
			});
			
			box.append(*label);
		}

		append(box);
	}
}

void layout::handle_keycode(
	Gtk::Label *label,
	const double &code,
	const bool &pressed) {

	auto style = label->get_style_context();

	// Shift, Ctrl
	if (code == 42 || code == 29) {
		if (!pressed)
			return;

		std::map<int, int> mod_map = {
			{42, 1},
			{29, 4}
		};

		if (style->has_class("toggled")) {
			style->remove_class("toggled");
			mods -= mod_map[code];
			window->set_modifier(mods);
			window->press_key(code, 0);
		}
		else {
			style->add_class("toggled");
			mods += mod_map[code];
			window->set_modifier(mods);
			window->press_key(code, 1);
		}
		return;
	}

	if (pressed) {
		style->add_class("pressed");
		window->press_key(code, 1);
	}
	else {
		style->remove_class("pressed");
		window->press_key(code, 0);
	}
}
