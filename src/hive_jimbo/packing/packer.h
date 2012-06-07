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

#include "packer_structures.h"

class JBPacker {
    private:
        JBPackerDirectory base_directory;

    public:
        JB_EXPORT JBPacker();
        JB_EXPORT ~JBPacker();
        JB_EXPORT void AddFile(std::string &file_path, std::fstream &file_stream);
        JB_EXPORT void RemoveFile(std::string &file_path);
        JB_EXPORT static void PackFile(std::string &file_path, std::string &target_path);
        JB_EXPORT static void UnpackFile(std::string &file_path, std::string &target_path);
};
