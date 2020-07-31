#pragma once
#include <Windows.h>            //allows use of windows api
#include <iostream>                //allows use of printing to console
#include <Psapi.h>                //header used by Process Status API (initially for patternscan.hpp)
#include <cstddef>                //defines several types implicitly generated or used (initally for hookmanager.hpp)
#include <assert.h>                //allows use of assert (initially for hookmanager.hpp)
#include <d3d9.h>                //used by Direct3D 9 Graphics
#include <string>                //allows use of c++ style strings
#include <math.h>                //allows use of basic math functions 
#include <vector>

#pragma warning(disable:4996)    //disables warning printing console
#pragma warning(disable:4244)    //gets rid of const int to float warning

#include "Utils/Utils.hpp"        //includes all utils...
#include "Globals.hpp"

//All necessary ImGui libraries
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"
#include "GUI/Config.hpp"
#include "GUI/GUI.hpp"

#include "Valve/Valve.hpp"

#include "Interfaces.hpp"
#include "Netvars.hpp"

#include "Hooks.hpp"            //hooking ingame functions
