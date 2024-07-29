#pragma once
#include <gtkmm/box.h>

class layout : public Gtk::Box {
	public:
		layout(std::vector<std::vector<std::string>> keymap);

	private:
		int btn_size = 70;
};

