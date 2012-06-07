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

#include "stdafx.h"

#ifndef JB_NO_ZLIB

#include "packer_gz.h"

JBPackerGz::JBPackerGz() : JBPacker() {
}

JBPackerGz::~JBPackerGz() {
}

void JBPackerGz::PackFile(std::string &file_path, std::string &target_path) {
    // allocates space for the buffer
    char buffer[GZ_BUFFER_SIZE];

    // creates the counter
    int count;

    // opens the file
    std::fstream file = std::fstream(file_path.c_str(), std::fstream::in | std::fstream::binary);

    // opens the target file
    gzFile target_file = gzopen(target_path.c_str(), "wb");

    // loops continually
    while(1) {
        // in case the end of file has been reached
        if(file.eof())
            break;

        // reads the file to the buffer
        file.read(buffer, GZ_BUFFER_SIZE);

        // retrieves the number of bytes read
        count = file.gcount();

        // writes the compresseed value
        gzwrite(target_file, buffer, count);
    }

    // closes the file
    file.close();

    // closes the target file
    gzclose(target_file);
}

void JBPackerGz::UnpackFile(std::string &file_path, std::string &target_path) {
    // allocates space for the buffer
    char buffer[GZ_BUFFER_SIZE];

    // creates the counter
    int count;

    // opens the file
    gzFile file = gzopen(file_path.c_str(), "rb");

    // opens the target file
    std::fstream target_file = std::fstream(target_path.c_str(), std::fstream::out | std::fstream::trunc | std::fstream::binary);

    // loops while the read is successful
    while((count = gzread(file, buffer, GZ_BUFFER_SIZE)) > 0) {
        // writes the uncompressed value
        target_file.write(buffer, count);
    }

    // closes the file
    gzclose(file);

    // closes the target file
    target_file.close();
}

#endif
