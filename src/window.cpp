#include "window.hpp"
#include "layout.hpp"
#include "keymap.tpp"
#include "os-compatibility.h"

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
}

static struct wl_registry_listener registry_listener = {
	&registry_handler
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
	std::vector<std::vector<std::string>> keymap = {
		{"q 16", "w 17", "e 18", "r 19", "t 20", "y 21", "u 22", "i 23", "o 24", "p 25"},
		{"a 30", "s 31", "d 32", "f 33", "g 34", "h 35", "j 36", "k 37", "l 38"},
		{"z 44", "x 45", "c 46", "v 47", "b 48", "n 49", "m 50"}
	};

	layout *layout_full = Gtk::make_managed<layout>(*this, keymap);
	set_child(*layout_full);

	auto gdk_display = gdk_display_get_default();
	auto display = gdk_wayland_display_get_wl_display(gdk_display);
	auto registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, this);
}

void sysboard::create_virtual_keyboard() {
	auto gdk_display = gdk_display_get_default();
	auto gdk_seat = gdk_display_get_default_seat(gdk_display);
	auto wl_seat = gdk_wayland_seat_get_wl_seat(gdk_seat);

	virtual_keyboard = zwp_virtual_keyboard_manager_v1_create_virtual_keyboard(
		keyboard_manager, wl_seat);

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