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

#include "stdafx.h"

#ifndef JIMBO_NO_ZLIB

#include "packer_gz.h"

CPackerGz::CPackerGz() : CPacker() {
}

CPackerGz::~CPackerGz() {
}

void CPackerGz::packFile(std::string &filePath, std::string &targetPath) {
    // allocates space for the buffer
    char buffer[GZ_BUFFER_SIZE];

    // creates the counter
    int count;

    // opens the file
    std::fstream file = std::fstream(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // opens the target file
    gzFile targetFile = gzopen(targetPath.c_str(), "wb");

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
        gzwrite(targetFile, buffer, count);
    }

    // closes the file
    file.close();

    // closes the target file
    gzclose(targetFile);
}

void CPackerGz::unpackFile(std::string &filePath, std::string &targetPath) {
    // allocates space for the buffer
    char buffer[GZ_BUFFER_SIZE];

    // creates the counter
    int count;

    // opens the file
    gzFile file = gzopen(filePath.c_str(), "rb");

    // opens the target file
    std::fstream targetFile = std::fstream(targetPath.c_str(), std::fstream::out | std::fstream::trunc | std::fstream::binary);

    // loops while the read is successful
    while((count = gzread(file, buffer, GZ_BUFFER_SIZE)) > 0) {
        // writes the uncompressed value
        targetFile.write(buffer, count);
    }

    // closes the file
    gzclose(file);

    // closes the target file
    targetFile.close();
}

#endif
