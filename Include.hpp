#pragma once
#pragma warning(disable:4244)    //gets rid of "conversion from 'x' to 'y', possible loss of data" warnings
#pragma warning(disable:6011)    //deref null pointer warning (super bugged and annoying)
/*
//these aren't really necessary
#pragma warning(disable:4996)    //disables warning printing console
#pragma warning(disable:4305)    //gets rid of argument truncation warnings
*/

#define _USE_MATH_DEFINES		 //to enable the use of mathmatecial constants   (like pi)

#include <Windows.h>             //allows use of windows api
#include <iostream>              //allows use of printing to console
#include <Psapi.h>               //header used by Process Status API (initially for patternscan.hpp)
#include <cstddef>               //defines several types implicitly generated or used (initally for hookmanager.hpp)
#include <assert.h>              //allows use of assert (initially for hookmanager.hpp)
#include <d3d9.h>                //used by Direct3D 9 Graphics
#include <string>                //allows use of c++ style strings
#include <math.h>                //allows use of basic math functions 
#include <vector>				 //for entity list
#include <deque>				 //allows use of deque ADT (initially used in backtracking) 
#include <mutex>				 //essential for thread safety (for http requests with GUI stuff)
#include <algorithm>			 //for use of std::clamp (initailly for entity.hpp)
#include <intrin.h>				 //for _ReturnAddress() (intitially for hooks.cpp)
#include <chrono>                // time
#include <map>                   // make config lookups fast
#include <initializer_list>      // for vararg functions

#include "Utils/Utils.hpp"       //includes all utils...

#include "Valve/Valve.hpp"
#include "Interfaces.hpp"
#include "Netvars.hpp"

#include "Globals.hpp"

// imgui
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "GUI/Config.hpp"
#include "GUI/GUI.hpp"

#include "Hacks/Hacks.hpp"

#include "Hooks.hpp"            //hooking ingame functions
