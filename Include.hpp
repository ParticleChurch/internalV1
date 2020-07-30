#pragma once
#include <Windows.h>			//allows use of windows api
#include <iostream>				//allows use of printing to console
#include <Psapi.h>				//header used by Process Status API (initially for patternscan.hpp)

#pragma warning(disable:4996)	//disables warning printing console

#include "Utils/Utils.hpp"		//includes all utils...

//All necessary ImGui librarys
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_impl_win32.h"