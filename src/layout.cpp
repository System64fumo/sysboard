#include "layout.hpp"
#include "layouts.hpp"
#include "window.hpp"
#include "key.hpp"

#include <algorithm>
#include <bitset>
#include <sys/time.h>

layout::layout(sysboard *win, const std::string &keymap_name) : Gtk::Grid() {
	window = win;
	mods = 0;
	last_shift_time = 0;
	shift_held = false;
	shift_temp = false;
	this->keymap_name = keymap_name;

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
	add_css_class(keymap_name);
	set_column_homogeneous(true);
	set_row_homogeneous(true);

	// Rows
	unsigned int row_counter = 0;
	for (ulong i = 0; i < keymap.size(); ++i) {
		int height = keymap[i].first;

		// Columns
		unsigned int col_counter = 0;
		for (ulong j = 0; j < keymap[i].second.size(); ++j) {
			std::istringstream iss(keymap[i].second[j]);
			unsigned int width;
			int code;
			std::string label;
			std::string label_shift;
			iss >> width >> code >> label >> label_shift;

			if (label == "Pad") {
				Gtk::Box* kbd_key = Gtk::make_managed<Gtk::Box>();
				attach(*kbd_key, col_counter, row_counter, width, height);
			}
			else {
				key* kbd_key = Gtk::make_managed<key>(code, label, label_shift);

				Glib::RefPtr<Gtk::GestureClick> gesture_click = Gtk::GestureClick::create();
				kbd_key->add_controller(gesture_click);

				// Handle events
				gesture_click->signal_pressed().connect([&, kbd_key](int, double, double) {
					handle_keycode(kbd_key, true);
				});
				gesture_click->signal_released().connect([&, kbd_key](int, double, double) {
					handle_keycode(kbd_key, false);
				});
				attach(*kbd_key, col_counter, row_counter, width, height);
			}
			col_counter += width;
		}
		row_counter += height;
	}
}

void layout::handle_keycode(key *kbd_key, const bool &pressed) {
	if (kbd_key == nullptr) {
		keymap_name = window->config_main["main"]["layout"];
		mods = 0;
		window->set_modifier(mods);

		for (auto& child : get_children())
			remove(*child);

		load();
		return;
	}

	auto style = kbd_key->get_style_context();
	bool is_shift = kbd_key->code == 42 || kbd_key->code == 54;

	if (is_shift) {
		if (pressed) {
			long current_time = get_time_in_us();
			long time_diff = current_time - last_shift_time;
			last_shift_time = current_time;

			if (shift_held) {
				// Release held shift
				shift_held = false;
				shift_temp = false;
				mods &= ~mod_map[kbd_key->code];
				style->remove_class("toggled");
			}
			else if (shift_temp && time_diff < 500000) {
				// Double tap: promote temp shift to shift hold
				shift_temp = false;
				shift_held = true;
				// Keep toggled class
			}
			else if (shift_temp) {
				// Single press when already temp: untoggle
				shift_temp = false;
				mods &= ~mod_map[kbd_key->code];
				style->remove_class("toggled");
			}
			else {
				// First press: temporary shift
				shift_temp = true;
				mods |= mod_map[kbd_key->code];
				style->add_class("toggled");
			}
		}

		window->set_modifier(mods);

		bool shift_active = mods & 1;
		for (auto& row : get_children()) {
			if (!row->has_css_class("key")) continue;
			auto row_key = static_cast<key*>(row);
			row_key->set_shift(shift_active);
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
			if (!row->has_css_class("key")) continue;
			auto row_key = static_cast<key*>(row);
			row_key->set_shift(shift_active);
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
		mods &= ~1; // Only remove shift modifier, not all modifiers
		window->set_modifier(mods);

		for (auto& row : get_children()) {
			if (!row->has_css_class("key")) continue;
			key* kbd_button = static_cast<key*>(row);
			kbd_button->set_shift(false);
			// Only remove toggled class from shift keys
			if (kbd_button->code == 42 || kbd_button->code == 54) {
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