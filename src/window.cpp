#include "window.hpp"
#include "layout.hpp"
#include "layouts.hpp"
#include "css.hpp"

#include <gtk4-layer-shell.h>

sysboard::sysboard(const config_board &cfg) {
	config_main = cfg;

	// Layer shell stuff
	gtk_layer_init_for_window(gobj());
	gtk_layer_set_namespace(gobj(), "sysboard");
	gtk_layer_set_layer(gobj(), GTK_LAYER_SHELL_LAYER_OVERLAY);
	gtk_layer_auto_exclusive_zone_enable(gobj());

	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_BOTTOM, true);
	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_LEFT, true);
	gtk_layer_set_anchor(gobj(), GTK_LAYER_SHELL_EDGE_RIGHT, true);

	// Initialization
	set_name("sysboard");
	initialize_protos();
	load_layout();

	// Load custom css
	std::string home_dir = getenv("HOME");
	std::string css_path = home_dir + "/.config/sys64/board/style.css";
	css_loader css(css_path, this);
}

void sysboard::load_layout() {
	auto monitors = gdk_display_get_monitors(gdk_display);
	auto monitor = GDK_MONITOR(g_list_model_get_item(monitors, 0));

	GdkRectangle geometry;
	gdk_monitor_get_geometry(monitor, &geometry);
	int max_width = geometry.width - (config_main.margin * 2);

	std::map<std::string, std::vector<std::vector<std::string>>> layout_map;
	layout_map["full"] = keymap_desktop;
	layout_map["mobile"] = keymap_mobile;

	layout *layout_board = Gtk::make_managed<layout>(this, layout_map[config_main.layout], max_width);
	set_child(*layout_board);
	layout_board->set_margin(config_main.margin);
}

extern "C" {
	sysboard *sysboard_create(const config_board &cfg) {
		return new sysboard(cfg);
	}
}
