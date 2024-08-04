#include "key.hpp"

key::key(const int &code, const std::string &label, const std::string &label_shift) {
	this->code = code;
	this->label = label;
	this->label_shift = label_shift;
	set_shift(false);
}

void key::set_shift(const bool &state) {
	if (state)
		set_text(label_shift);
	else
		set_text(label);
}
