#pragma once
// Use this to create a new layout: https://github.com/torvalds/linux/blob/master/include/uapi/linux/input-event-codes.h
// Order: Width Code Label Label_Shift

std::vector<std::pair<int, std::vector<std::string>>> keymap_desktop = {
	{1, {"4 1 Esc Esc", "4 2 1 !", "4 3 2 @", "4 4 3 #", "4 5 4 $", "4 6 5 %", "4 7 6 ^", "4 8 7 &", " 4 9 8 *", "4 10 9 (", "4 11 0 )", "4 12 - _", "4 13 = +", "8 14 Backspace Backspace"}},
	{1, {"5 15 Tab Tab", "4 16 q Q", "4 17 w W", "4 18 e E", "4 19 r R", "4 20 t T", "4 21 y Y", "4 22 u U", "4 23 i I", "4 24 o O", "4 25 p P", "4 26 [ {", "4 27 ] }", "7 43 \\ |"}},
	{1, {"6 58 Caps Caps", "4 30 a A", "4 31 s S", "4 32 d D", "4 33 f F", "4 34 g G", "4 35 h H", "4 36 j J", "4 37 k K ", "4 38 l L", "4 39 ; :", "4 40 ' \"", "10 28 Enter Enter"}},
	{1, {"8 42 Shift Shift", "4 44 z Z", "4 45 x X", "4 46 c C", "4 47 v V", "4 48 b B", "4 49 n N", "4 50 m M", "4 51 , <", "4 52 . >", "4 53 / ?", "12 54 Shift Shift"}},
	{1, {"5 29 Ctrl Ctrl", "5 56 Alt Alt", "5 125 Meta Meta", "29 57 Space Space", "4 105 ← ←", "4 103 ↑ ↑", "4 108 ↓ ↓", "4 106 → →"}}
};

std::vector<std::pair<int, std::vector<std::string>>> keymap_mobile = {
	{2, {"3 1 Esc Esc", "3 15 Tab Tab", "2 29 Ctrl Ctrl", "2 56 Alt Alt", "2 53 / ?", "2 105 ← ←", "2 103 ↑ ↑", "2 108 ↓ ↓", "2 106 → →"}},
	{3, {"2 16 q Q", "2 17 w W", "2 18 e E", "2 19 r R", "2 20 t T", "2 21 y Y", "2 22 u U", "2 23 i I", "2 24 o O", "2 25 p P"}},
	{3, {"1 0 Pad Pad", "2 30 a A", "2 31 s S", "2 32 d D", "2 33 f F", "2 34 g G", "2 35 h H", "2 36 j J", "2 37 k K ", "2 38 l L", "1 0 Pad Pad"}},
	{3, {"3 42 Shift Shift", "2 44 z Z", "2 45 x X", "2 46 c C", "2 47 v V", "2 48 b B", "2 49 n N", "2 50 m M", "3 14 Backspace Backspace"}},
	{3, {"5 0 123 123", "10 57 Space Space", "5 28 Enter Enter"}}
};

std::vector<std::pair<int, std::vector<std::string>>> keymap_mobile_numbers = {
	{2, {"3 1 Esc Esc", "3 15 Tab Tab", "2 29 Ctrl Ctrl", "2 56 Alt Alt", "2 53 / ?", "2 105 ← ←", "2 103 ↑ ↑", "2 108 ↓ ↓", "2 106 → →"}},
	{3, {"2 2 1 !", "2 3 2 @", "2 4 3 #", "2 5 4 $", "2 6 5 %", "2 7 6 ^", "2 8 7 &", " 2 9 8 *", "2 10 9 (", "2 11 0 )"}},
	{3, {"2 0 Pad Pad", "2 12 - _", "2 13 = +", "2 26 [ {", "2 27 ] }", "2 43 \\ |", "2 39 ; :", "2 40 ' \"", "2 41 ` ~", "2 0 Pad Pad"}},
	{3, {"3 42 Shift Shift", "4 0 Pad Pad", "2 51 , <", "2 52 . >", "2 53 / ?", "4 0 Pad Pad", "3 14 Backspace Backspace"}},
	{3, {"5 0 abc abc", "10 57 Space Space", "5 28 Enter Enter"}}
};
