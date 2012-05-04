// Hive Jimbo Library
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Jimbo Library.
//
// Hive Jimbo Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Jimbo Library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Jimbo Library. If not, see <http://www.gnu.org/licenses/>.
//
// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#ifdef _DEBUG
#define JB_DEBUG
#endif

#ifdef NO_ZLIB
#define JB_NO_ZLIB
#endif

#define JB_EXPORT __declspec(dllexport)
#define JB_NO_EXPORT

#include "global/targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <direct.h>
#include <shlobj.h>
#include <windows.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <winsock2.h>
#include <tlhelp32.h>

#include <ctime>
#include <string>

#include <iostream>
#include <sstream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <list>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

#ifndef JB_NO_ZLIB
#include <zlib.h>
#ifdef JB_DEBUG
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif
#endif
