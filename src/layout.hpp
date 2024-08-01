#pragma once
#include "window.hpp"

#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/gestureclick.h>

class layout : public Gtk::Box {
	public:
		layout(sysboard *win, std::vector<std::vector<std::string>> keymap);

	private:
		sysboard *window;
		double btn_size = 70;

		void handle_keycode(
			Gtk::Label *label,
			const double &code,
			const bool &pressed);
};

