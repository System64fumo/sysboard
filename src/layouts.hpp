#pragma once
// Use this to create a new layout: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
// Order: Size Code Label Label_Shift

std::vector<std::vector<std::string>> keymap_desktop = {
	{"1 1 Esc Esc", "1 2 1 !", "1 3 2 @", "1 4 3 #", "1 5 4 $", "1 6 5 %", "1 7 6 ^", "1 8 7 &", " 1 9 8 *", "1 10 9 (", "1 11 0 )", "1 12 - _", "1 13 = +", "3 14 Backspace Backspace"},
	{"2 15 Tab Tab", "1 16 q Q", "1 17 w W", "1 18 e E", "1 19 r R", "1 20 t T", "1 21 y Y", "1 22 u U", "1 23 i I", "1 24 o O", "1 25 p P", "1 26 [ {", "1 27 ] }", "2 43 \\ |"},
	{"2 58 Caps Caps", "1 30 a A", "1 31 s S", "1 32 d D", "1 33 f F", "1 34 g G", "1 35 h H", "1 36 j J", "1 37 k K ", "1 38 l L", "1 39 ; :", "1 40 ' \"", "3 28 Enter Enter"},
	{"3 42 Shift Shift", "1 44 z Z", "1 45 x X", "1 46 c C", "1 47 v V", "1 48 b B", "1 49 n N", "1 50 m M", "1 51 , <", "1 52 . >", "1 53 / ?", "3 54 Shift Shift"},
	{"1 29 Ctrl Ctrl", "1 56 Alt Alt", "1 125 Meta Meta", "9 57 Space Space", "1 105 ← ←", "1 103 ↑ ↑", "1 108 ↓ ↓", "1 106 → →"},
};

std::vector<std::vector<std::string>> keymap_mobile = {
	{"1 1 Esc Esc", "1 15 Tab Tab", "1 29 Ctrl Ctrl", "1 125 Meta Meta", "1 56 Alt Alt", "1 53 / ?", "1 105 ← ←", "1 103 ↑ ↑", "1 108 ↓ ↓", "1 106 → →"},
	{"1 16 q Q", "1 17 w W", "1 18 e E", "1 19 r R", "1 20 t T", "1 21 y Y", "1 22 u U", "1 23 i I", "1 24 o O", "1 25 p P"},
	{"1 30 a A", "1 31 s S", "1 32 d D", "1 33 f F", "1 34 g G", "1 35 h H", "1 36 j J", "1 37 k K ", "1 38 l L"},
	{"1.5 42 Shift Shift", "1 44 z Z", "1 45 x X", "1 46 c C", "1 47 v V", "1 48 b B", "1 49 n N", "1 50 m M", "1.5 14 Backspace Backspace"},
	{"2 0 123 123", "6 57 Space Space", "2 28 Enter Enter"},
};

std::vector<std::vector<std::string>> keymap_mobile_numbers = {
	{"1 1 Esc Esc", "1 15 Tab Tab", "1 29 Ctrl Ctrl", "1 125 Meta Meta", "1 56 Alt Alt", "1 53 / ?", "1 105 ← ←", "1 103 ↑ ↑", "1 108 ↓ ↓", "1 106 → →"},
	{"1 2 1 !", "1 3 2 @", "1 4 3 #", "1 5 4 $", "1 6 5 %", "1 7 6 ^", "1 8 7 &", " 1 9 8 *", "1 10 9 (", "1 11 0 )"},
	{"1 12 - _", "1 13 = +", "1 26 [ {", "1 27 ] }", "1 43 \\ |", "1 39 ; :", "1 40 ' \"", "1 41 ` ~"},
	{"1.5 42 Shift Shift", "1 51 , <", "1 52 . >", "1 53 / ?", "1.5 14 Backspace Backspace"},
	{"2 0 abc abc", "6 57 Space Space", "2 28 Enter Enter"},
};
