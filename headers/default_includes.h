#pragma once

#include <exception>
#include <string>
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <cmath>
#include <vector>

//dependencies
#include <SDL3/SDL.h>

#ifndef ENABLE_DEBUG_TEXT
#define ENABLE_DEBUG_TEXT
#endif

#ifndef DEBUG_PRINT
#ifdef ENABLE_DEBUG_TEXT
#define DEBUG_PRINT(text) std::cout << "[DEBUG] " << text << std::endl
#else
#define DEBUG_PRINT(text)
#endif
#endif