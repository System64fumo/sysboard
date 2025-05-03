#include "layout.hpp"
#include "layouts.hpp"
#include "window.hpp"
#include "key.hpp"

#include <algorithm>
#include <bitset>

layout::layout(sysboard *win, const std::string &keymap_name, const int &max_width) : Gtk::Box(Gtk::Orientation::VERTICAL) {
	window = win;
	this->keymap_name = keymap_name;
	this->max_width = max_width;
	set_halign(Gtk::Align::CENTER);

	// Layouts
	layout_map["full"] = keymap_desktop;
	layout_map["mobile"] = keymap_mobile;
	layout_map["mobile_numbers"] = keymap_mobile_numbers;

	// Modifiers
	mod_map[42] = 1;	// Left Shift
	mod_map[54] = 1;	// Right Shift
	mod_map[29] = 4;	// Ctrl
	mod_map[56] = 8;	// Alt
	mod_map[125] = 4;	// Meta

	load();
}

void layout::load() {
	keymap = layout_map[keymap_name];

	// Dynamic scaling
	auto largest_vec_it = std::max_element(
		keymap.begin(), keymap.end(),
		[](const auto& a, const auto& b) {
			return a.size() < b.size();
		}
	);
	btn_size = max_width / largest_vec_it->size();

	// Get the widest row
	double pixels = 0;
	for (const std::string& str : *largest_vec_it) {
		std::istringstream iss(str);
		double multiplier;
		int code;
		std::string label;
		std::string label_shift;
		iss >> multiplier >> code >> label >> label_shift;

		pixels += btn_size * multiplier;
	}

	// Scale the button
	double scaling_factor = max_width / pixels;
	btn_size = btn_size * scaling_factor;

	// Rows
	for (ulong i = 0; i < keymap.size(); ++i) {
		Gtk::Box box = Gtk::Box(Gtk::Orientation::HORIZONTAL);

		// Keys
		for (ulong j = 0; j < keymap[i].size(); ++j) {
			std::istringstream iss(keymap[i][j]);
			double multiplier;
			int code;
			std::string label;
			std::string label_shift;
			iss >> multiplier >> code >> label >> label_shift;

			key *kbd_key = Gtk::make_managed<key>(code, label, label_shift);
			kbd_key->set_focusable(false);
			kbd_key->set_size_request(btn_size * multiplier, btn_size * std::stod(window->config_main["main"]["height-multiplier"]));

			Glib::RefPtr<Gtk::GestureClick> gesture_click = Gtk::GestureClick::create();
			kbd_key->add_controller(gesture_click);

			// Handle events
			gesture_click->signal_pressed().connect([&, kbd_key](int, double, double) {
				handle_keycode(kbd_key, true);
			});
			gesture_click->signal_released().connect([&, kbd_key](int, double, double) {
				handle_keycode(kbd_key, false);
			});

			box.append(*kbd_key);

			// Starting key
			if (j == 0) {
				kbd_key->set_hexpand(true);
				kbd_key->set_halign((multiplier == 1) ? Gtk::Align::END : Gtk::Align::START);
			}
			// Ending key
			else if (j == keymap[i].size() - 1) {
				kbd_key->set_hexpand(true);
				kbd_key->set_halign((multiplier == 1) ? Gtk::Align::START : Gtk::Align::END);
			}
		}

		append(box);
	}
}

void layout::handle_keycode(key *kbd_key, const bool &pressed) {
	auto style = kbd_key->get_style_context();
	bool is_shift = kbd_key->code == 42 || kbd_key->code == 54;

	if (is_shift && pressed) {
		long current_time = get_time_in_us();
		long time_diff = current_time - last_shift_time;
		last_shift_time = current_time;

		// Disable shift hold
		if (shift_held) {
			shift_held = false;
			mods &= ~mod_map[kbd_key->code];
			style->remove_class("toggled");
		}
		// Promote temp shift to shift hold
		else if (shift_temp && time_diff < 500000) {
			shift_temp = false;
			shift_held = true;
		}
		// Enable temporary shift
		else {
			shift_temp = true;
			mods |= mod_map[kbd_key->code];
			style->add_class("toggled");
		}

		window->set_modifier(mods);

		bool shift_active = mods & 1;
		for (auto& row : get_children()) {
			for (auto& row_child : row->get_children()) {
				static_cast<key*>(row_child)->set_shift(shift_active);
			}
		}
		return;
	}

	// Handle other modifiers
	if (mod_map.find(kbd_key->code) != mod_map.end()) {
		if (!pressed)
			return;

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

		bool shift_active = mods & 1;
		for (auto& row : get_children()) {
			for (auto& row_child : row->get_children()) {
				static_cast<key*>(row_child)->set_shift(shift_active);
			}
		}
		return;
	}

	// Normal key press/release
	if (pressed) {
		style->add_class("pressed");
		window->press_key(kbd_key->code, 1);
	}
	else {
		style->remove_class("pressed");
		window->press_key(kbd_key->code, 0);
	}

	// Clear temporary shift after one use
	if (shift_temp && !shift_held) {
		shift_temp = false;
		mods = 0;
		window->set_modifier(mods);

		for (auto& row : get_children()) {
			for (auto& row_child : row->get_children()) {
				key* kbd_button = static_cast<key*>(row_child);
				kbd_button->set_shift(false);
				kbd_button->get_style_context()->remove_class("toggled");
			}
		}
	}

	// Handle layout switching (e.g. 123/abc key)
	if (!pressed && kbd_key->code == 0) {
		if (kbd_key->label == "123")
			keymap_name = "mobile_numbers";
		else if (kbd_key->label == "abc")
			keymap_name = "mobile";

		for (auto& child : get_children())
			remove(*child);

		mods = 0;
		window->set_modifier(mods);
		load();
	}
}

long layout::get_time_in_us() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (tv.tv_sec * 1000000 + tv.tv_usec);
}