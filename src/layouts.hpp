#pragma once
// Use this to create a new layout: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
// Order: Label Code Size

std::vector<std::vector<std::string>> keymap_desktop = {
	{"Esc 1 1", "1 2 1", "2 3 1", "3 4 1", "4 5 1", "5 6 1", "6 7 1", "7 8 1", "8 9 1", "9 10 1", "0 11 1", "- 12 1", "= 13 1", "Backspace 14 3"},
	{"Tab 15 2", "q 16 1", "w 17 1", "e 18 1", "r 19 1", "t 20 1", "y 21 1", "u 22 1", "i 23 1", "o 24 1", "p 25 1", "[ 26 1", "] 27 1", "\\ 43 2"},
	{"Caps 58 2", "a 30 1", "s 31 1", "d 32 1", "f 33 1", "g 34 1", "h 35 1", "j 36 1", "k 37 1", "l 38 1", "; 39 1", "' 40 1", "Enter 28 3"},
	{"Shift 42 3", "z 44 1", "x 45 1", "c 46 1", "v 47 1", "b 48 1", "n 49 1", "m 50 1", ", 51 1", ". 52 1", "/ 53 1", "Shift 54 3"},
	{"Ctrl 29 1", "Alt 56 1", "Meta 125 1", "Space 57 9", "← 105 1", "↑ 103 1", " ↓ 108 1", "→ 106 1"},
};

std::vector<std::vector<std::string>> keymap_mobile = {
	{"Esc 1 1", "Tab 15 1", "Ctrl 29 1", "Meta 125 1", "Alt 56 1", "/ 53 1", "← 105 1", "↑ 103 1", " ↓ 108 1", "→ 106 1"},
	{ "q 16 1", "w 17 1", "e 18 1", "r 19 1", "t 20 1", "y 21 1", "u 22 1", "i 23 1", "o 24 1", "p 25 1"},
	{"a 30 1", "s 31 1", "d 32 1", "f 33 1", "g 34 1", "h 35 1", "j 36 1", "k 37 1", "l 38 1"},
	{"Shift 42 1.5", "z 44 1", "x 45 1", "c 46 1", "v 47 1", "b 48 1", "n 49 1", "m 50 1", "Backspace 14 1.5"},
	{"123 0 2", "Space 57 6", "Enter 28 2"},
};
