#pragma once
#include "window.hpp"

#include <gtkmm/box.h>

class layout : public Gtk::Box {
	public:
		layout(const sysboard &window, std::vector<std::vector<std::string>> keymap);

	private:
		double btn_size = 70;
};

