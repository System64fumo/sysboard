#pragma once
#include "window.hpp"

config_board config_main;
typedef sysboard* (*sysboard_create_func)(const config_board &cfg);
sysboard_create_func sysboard_create_ptr;
