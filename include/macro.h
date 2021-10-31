#pragma once

#define DEBUG

#ifdef DEBUG
#include <iostream>
#define M_DEBUG(msg) std::cout << msg << std::endl;
#else
#define M_DEBUG(msg)
#endif