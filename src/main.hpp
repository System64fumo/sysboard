#pragma once
#include "config.hpp"

class sysboard;
sysboard* window;

config_board config_main;

typedef sysboard* (*sysboard_create_func)(const config_board&);
sysboard_create_func sysboard_create_ptr;

typedef void (*sysboard_handle_signal_func)(sysboard*, int);
sysboard_handle_signal_func sysboard_handle_signal_ptr;
