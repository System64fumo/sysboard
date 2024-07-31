#include "window.hpp"
#include "layout.hpp"
#include "keymap.tpp"
#include "os-compatibility.h"
#include "css.hpp"

#include <gdk/wayland/gdkwayland.h>
#include <gtk4-layer-shell.h>
#include <iostream>
#include <sys/mman.h>
#include <cstring>

static void registry_handler(void *data, struct wl_registry *registry,
							 uint32_t id, const char *interface, uint32_t version) {

	auto self = static_cast<sysboard*>(data);

	if (strcmp(interface, zwp_virtual_keyboard_manager_v1_interface.name) == 0) {
		self->keyboard_manager = (zwp_virtual_keyboard_manager_v1*)
			wl_registry_bind(registry, id,
				&zwp_virtual_keyboard_manager_v1_interface, 1u);
		self->create_virtual_keyboard();
	}
	else if (strcmp(interface, zwp_input_method_manager_v2_interface.name) == 0) {
		self->input_method_manager = (zwp_input_method_manager_v2*)
			wl_registry_bind(registry, id,
				&zwp_input_method_manager_v2_interface, 1u);
		self->create_input_manager();
	}
}

static wl_registry_listener registry_listener = {
	&registry_handler
};

static void input_method_activate(void *data, struct zwp_input_method_v2 *zwp_input_method_v2) {
	auto self = static_cast<sysboard*>(data);
	self->show();
}

static void input_method_deactivate(void *data, struct zwp_input_method_v2 *zwp_input_method_v2) {
	auto self = static_cast<sysboard*>(data);
	self->hide();
}

static void input_method_surrounding_text(void *data,
				struct zwp_input_method_v2 *zwp_input_method_v2,
				const char *text,
				uint32_t cursor,
				uint32_t anchor) {
	// I could probably use this for autocomplete/autocorrect
}

static void input_method_text_change_cause(void *data,
				struct zwp_input_method_v2 *zwp_input_method_v2,
				uint32_t cause) {}

static void input_method_content_type(void *data,
				struct zwp_input_method_v2 *zwp_input_method_v2,
				uint32_t hint,
				uint32_t purpose) {}

static void input_method_done(void *data, struct zwp_input_method_v2 *zwp_input_method_v2) {
	auto wl_display = wl_display_connect(NULL);
	wl_display_roundtrip(wl_display);
}

static void input_method_unavailable(void *data, struct zwp_input_method_v2 *zwp_input_method_v2) {}

static zwp_input_method_v2_listener input_method_listener = {
	.activate = input_method_activate,
	.deactivate = input_method_deactivate,
	.surrounding_text = input_method_surrounding_text,
	.text_change_cause = input_method_text_change_cause,
	.content_type = input_method_content_type,
	.done = input_method_done,
	.unavailable = input_method_unavailable,
};

sysboard::sysboard() {
	// Layer shell stuff
	gtk_layer_init_for_window(gobj());
	gtk_layer_set_keyboard_mode(gobj(), GTK_LAYER_SHELL_KEYBOARD_MODE_NONE);
	gtk_layer_set_namespace(gobj(), "sysboard");
	gtk_layer_set_layer(gobj(), GTK_LAYER_SHELL_LAYER_OVERLAY);
	gtk_layer_auto_exclusive_zone_enable(gobj());

	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_RIGHT, true);

	// Initialization
	set_name("sysboard");
	show();

	// Temporary
	// Use this to create a new layout: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
	// Order: Label Code Size
	std::vector<std::vector<std::string>> keymap = {
		{"Esc 1 1", "1 2 1", "2 3 1", "3 4 1", "4 5 1", "5 6 1", "6 7 1", "7 8 1", "8 9 1", "9 10 1", "0 11 1", "- 12 1", "= 13 1", "Backspace 14 3"},
		{"Tab 15 2", "q 16 1", "w 17 1", "e 18 1", "r 19 1", "t 20 1", "y 21 1", "u 22 1", "i 23 1", "o 24 1", "p 25 1", "[ 26 1", "] 27 1", "\\ 43 2"},
		{"Caps 58 2", "a 30 1", "s 31 1", "d 32 1", "f 33 1", "g 34 1", "h 35 1", "j 36 1", "k 37 1", "l 38 1", "; 39 1", "' 40 1", "Enter 28 3"},
		{"Shift 42 3", "z 44 1", "x 45 1", "c 46 1", "v 47 1", "b 48 1", "n 49 1", "m 50 1", ", 51 1", ". 52 1", "/ 53 1", "Shift 54 3"},
		{"Ctrl 29 1", "Alt 56 1", "Meta 125 1", "Space 57 12", "Alt 100 1"},
	};

	layout *layout_full = Gtk::make_managed<layout>(*this, keymap);
	set_child(*layout_full);
	layout_full->set_margin(10);

	gdk_display = gdk_display_get_default();
	gdk_seat = gdk_display_get_default_seat(gdk_display);
	seat = gdk_wayland_seat_get_wl_seat(gdk_seat);
	auto display = gdk_wayland_display_get_wl_display(gdk_display);
	auto registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, this);

	// Load custom css
	std::string home_dir = getenv("HOME");
	std::string css_path = home_dir + "/.config/sys64/board/style.css";
	css_loader css(css_path, this);
}

void sysboard::create_virtual_keyboard() {
	virtual_keyboard = zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
		keyboard_manager, seat);

	size_t keymap_size = strlen(keymap) + 1;
	int keymap_fd = os_create_anonymous_file(keymap_size);
	void *ptr = mmap(nullptr, keymap_size, PROT_READ | PROT_WRITE, MAP_SHARED, keymap_fd, 0);
	std::strcpy((char*)ptr, keymap);

	zwp_virtual_keyboard_v1_keymap(
		virtual_keyboard,
		WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1,
		keymap_fd,
		keymap_size);
}

void sysboard::create_input_manager() {
	input_method = zwp_input_method_manager_v2_get_input_method(input_method_manager, seat);
	zwp_input_method_v2_add_listener(input_method, &input_method_listener, this);
}

void sysboard::press_key(int keycode, int state) const {
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	guint32 time = ts.tv_sec * 1000ll + ts.tv_nsec / 1000000ll;
	zwp_virtual_keyboard_v1_key(virtual_keyboard, time, keycode, state);
}

extern "C" {
	sysboard *sysboard_create() {
		return new sysboard();
	}
}
