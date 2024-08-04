#include "layout.hpp"

#include <algorithm>

layout::layout(sysboard *win, std::vector<std::vector<std::string>> keymap, const int &max_width) : Gtk::Box(Gtk::Orientation::VERTICAL) {
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

	btn_size = max_width / largest_vec_it->size();

	double pixels = 0;

	for (const std::string& str : *largest_vec_it) {
		std::istringstream iss(str);
		std::string text;
		int code;
		double multiplier;
		iss >> text >> code >> multiplier;

		pixels += btn_size * multiplier;
	}

	double scaling_factor = max_width / pixels;
	btn_size = btn_size * scaling_factor;

	// Populate layout
	for (ulong i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);
		box.set_halign(Gtk::Align::CENTER);

		for (ulong j = 0; j < keymap[i].size(); ++j) {
			std::istringstream iss(keymap[i][j]);
			std::string text;
			int code;
			double multiplier;
			iss >> text >> code >> multiplier;

			key *kbd_key = Gtk::make_managed<key>(code, text, text);
			kbd_key->set_focusable(false);
			kbd_key->set_size_request(btn_size * multiplier, btn_size * height_multiplier);

			Glib::RefPtr<Gtk::GestureClick> gesture_click = Gtk::GestureClick::create();
			kbd_key->add_controller(gesture_click);

			// Handle events
			// TODO: Handle special events (code 0) based on their label
			// TODO: Toggle mod keys
			gesture_click->signal_pressed().connect([&, kbd_key](int, double, double) {
				handle_keycode(kbd_key, true);
			});
			gesture_click->signal_released().connect([&, kbd_key](int, double, double) {
				handle_keycode(kbd_key, false);
			});
			box.append(*kbd_key);
		}

		append(box);
	}
}

// Should this be moved to the key itself?
// Wouldn't that use more resources?
void layout::handle_keycode(
	key *kbd_key,
	const bool &pressed) {

	auto style = kbd_key->get_style_context();

	// Shift, Ctrl
	if (kbd_key->code == 42 || kbd_key->code == 29) {
		if (!pressed)
			return;

		std::map<int, int> mod_map = {
			{42, 1},
			{29, 4}
		};

		if (style->has_class("toggled")) {
			style->remove_class("toggled");
			mods -= mod_map[kbd_key->code];
			window->set_modifier(mods);
			window->press_key(kbd_key->code, 0);
		}
		else {
			style->add_class("toggled");
			mods += mod_map[kbd_key->code];
			window->set_modifier(mods);
			window->press_key(kbd_key->code, 1);
		}
		return;
	}

	if (pressed) {
		style->add_class("pressed");
		window->press_key(kbd_key->code, 1);
	}
	else {
		style->remove_class("pressed");
		window->press_key(kbd_key->code, 0);
	}
}
