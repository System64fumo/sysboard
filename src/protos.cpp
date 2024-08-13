#include "window.hpp"
#include "keymap.tpp"
#include "os-compatibility.h"

#include <gdk/wayland/gdkwayland.h>
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
	if (!self->manual_mode)
		self->show();
}

static void input_method_deactivate(void *data, struct zwp_input_method_v2 *zwp_input_method_v2) {
	auto self = static_cast<sysboard*>(data);
	if (!self->manual_mode)
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
				uint32_t purpose) {
	auto self = static_cast<sysboard*>(data);
	if (purpose == 9) // Pin
		self->hide();
}

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

void sysboard::initialize_protos() {
	gdk_display = gdk_display_get_default();
	gdk_seat = gdk_display_get_default_seat(gdk_display);
	seat = gdk_wayland_seat_get_wl_seat(gdk_seat);
	auto display = gdk_wayland_display_get_wl_display(gdk_display);
	auto registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, this);
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

void sysboard::press_key(const int &keycode, const int &state) {
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	guint32 time = ts.tv_sec * 1000ll + ts.tv_nsec / 1000000ll;
	zwp_virtual_keyboard_v1_key(virtual_keyboard, time, keycode, state);
}

void sysboard::set_modifier(const int &mod) {
	zwp_virtual_keyboard_v1_modifiers(virtual_keyboard,
		mod, 0, 0, 0);
}
