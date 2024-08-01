#include "window.hpp"
#include "layout.hpp"
#include "layouts.hpp"
#include "css.hpp"

#include <gtk4-layer-shell.h>
#include <iostream>

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
	initialize_protos();

	layout *layout_full = Gtk::make_managed<layout>(this, keymap_desktop);
	set_child(*layout_full);
	layout_full->set_margin(10);

	// Load custom css
	std::string home_dir = getenv("HOME");
	std::string css_path = home_dir + "/.config/sys64/board/style.css";
	css_loader css(css_path, this);
}

extern "C" {
	sysboard *sysboard_create() {
		return new sysboard();
	}
}
