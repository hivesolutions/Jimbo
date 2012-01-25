/*
 Hive Jimbo Library
 Copyright (C) 2008 Hive Solutions Lda.

 This file is part of Hive Jimbo Library.

 Hive Jimbo Library is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Hive Jimbo Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Hive Jimbo Library. If not, see <http://www.gnu.org/licenses/>.

 __author__    = João Magalhães <joamag@hive.pt>
 __version__   = 1.0.0
 __revision__  = $LastChangedRevision$
 __date__      = $LastChangedDate$
 __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
 __license__   = GNU General Public License (GPL), Version 3
*/

#pragma once

#ifdef NO_ZLIB
#define JIMBO_NO_ZLIB
#endif

#define JIMBO_EXPORT_PREFIX __declspec(dllexport)
#define JIMBO_NO_EXPORT_PREFIX

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <direct.h>
#include <windows.h>
#include <winsock2.h>

#include <string>

#include <iostream>
#include <sstream>
#include <fstream>

#include <algorithm>
#include <vector>
#include <list>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

#ifndef JIMBO_NO_ZLIB
#include <zlib.h>
#pragma comment(lib, "zlib1.lib")
#endif
