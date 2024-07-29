#pragma once
#include "window.hpp"

typedef sysboard* (*sysboard_create_func)();
sysboard_create_func sysboard_create_ptr;
