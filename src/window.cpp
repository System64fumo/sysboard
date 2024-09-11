#include "window.hpp"
#include "layout.hpp"
#include "css.hpp"

#include <gtk4-layer-shell.h>
#include <filesystem>
#include <glibmm/main.h>

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
	std::string style_path;
	if (std::filesystem::exists(std::string(getenv("HOME")) + "/.config/sys64/board/style.css"))
		style_path = std::string(getenv("HOME")) + "/.config/sys64/board/style.css";
	else if (std::filesystem::exists("/usr/share/sys64/board/style.css"))
		style_path = "/usr/share/sys64/board/style.css";
	else
		style_path = "/usr/local/share/sys64/board/style.css";
	css_loader css(style_path, this);
}

void sysboard::load_layout() {
	auto monitors = gdk_display_get_monitors(gdk_display);
	auto monitor = GDK_MONITOR(g_list_model_get_item(monitors, 0));

	GdkRectangle geometry;
	gdk_monitor_get_geometry(monitor, &geometry);
	int max_width = geometry.width - (config_main.margin * 2);

	layout *layout_board = Gtk::make_managed<layout>(this, config_main.layout, max_width);
	set_child(*layout_board);
	layout_board->set_margin(config_main.margin);
}

void sysboard::handle_signal(const int &signum, const bool& manual) {
	// Timeout exists to prevent a ping pong effect
	// Currently it's set to 250ms altho 100ms also works well enough
	timeout_connection.disconnect();
	timeout_connection = Glib::signal_timeout().connect([&, signum, manual]() {
		Glib::signal_idle().connect([&, signum, manual]() {

			// Reset all active modifiers to prevent weird behavior
			set_modifier(0);

			if (signum == 10) // Show
				show();

			else if (signum == 12) // Hide
				hide();

			else if (signum == 34) // Toggle
				set_visible(!manual_mode);

			if (manual)
				manual_mode = get_visible();

			return false;
		});
		return false;
	}, 250);
}

extern "C" {
	sysboard *sysboard_create(const config_board &cfg) {
		return new sysboard(cfg);
	}
	void sysboard_signal(sysboard *window, int signal) {
		window->handle_signal(signal, true);
	}
}
