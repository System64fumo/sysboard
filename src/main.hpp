#pragma once
#include <map>
#include <string>

class sysboard;
sysboard* window;

typedef sysboard* (*sysboard_create_func)(const std::map<std::string, std::map<std::string, std::string>>&);
sysboard_create_func sysboard_create_ptr;

typedef void (*sysboard_handle_signal_func)(sysboard*, int);
sysboard_handle_signal_func sysboard_handle_signal_ptr;
