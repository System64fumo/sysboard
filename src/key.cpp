#include "key.hpp"

key::key(const int &code, const std::string &label, const std::string &label_shift) : code(code), label(label), label_shift(label_shift) {
	append(label_main);
	label_main.set_hexpand(true);

	// TODO: Add different handling for special keys
	add_css_class("key-" + label);
	add_css_class("key");
	set_shift(false);
}

void key::set_shift(const bool &state) {
	label_main.set_text(state ? label_shift : label);
}
