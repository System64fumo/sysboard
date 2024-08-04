#pragma once
#include <gtkmm/label.h>

class key : public Gtk::Label {
	public:
		key(const int &code, const std::string &label, const std::string &label_shift);

		int code;
		std::string label;
		std::string label_shift;
};
