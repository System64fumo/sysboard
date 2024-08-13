#pragma once
#include "window.hpp"

config_board config_main;
typedef sysboard* (*sysboard_create_func)(const config_board &cfg);
typedef void (*sysboard_handle_signal_func)(sysboard*, int);
sysboard_create_func sysboard_create_ptr;
sysboard_handle_signal_func sysboard_handle_signal_ptr;
sysboard *window;
