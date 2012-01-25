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

#include "packer_tar.h"

CPackerTar::CPackerTar() : CPacker() {
}

CPackerTar::~CPackerTar() {
}

void CPackerTar::packFile(std::string &filePath, std::string &targetPath) {
}

void CPackerTar::unpackFile(std::string &filePath, std::string &targetPath) {
    // creates the counter
    int count;

    // creates the read counter
    int readCount;

    // creates space for the size
    unsigned long int sizeInt;

    // creates space for the size remaining
    unsigned long int sizeRemaining;

    // creates space for the remainder value
    int remainderValue;

    // allocates space for the header buffer
    char baseBuffer[TAR_BUFFER_BASE];

    // allocates space for the buffer
    char buffer[TAR_BUFFER_SIZE];

    // allocates space for the next file name buffer
    char nextFileNameBuffer[LARGE_NAME_SIZE];

    // creates the valid next file name flag
    bool validNextFileName = false;

    // the target file pointer
    std::fstream *targetFile;

    // opens the file
    std::fstream file = std::fstream(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // loops continually
    while(1) {
        // in case the end of file has been reached
        if(file.eof())
            break;

        // reads the header of the tar file
        file.read(baseBuffer, TAR_BUFFER_BASE);

        // retrieves the buffer header for the read buffer
        tarHeader *bufferHeader = (tarHeader *) baseBuffer;

        // retrieves the file size
        sscanf_s(bufferHeader->size, "%lo", &sizeInt);

        // starts the file path with the target path
        std::string filePath = targetPath;

        // in case the flag valis next file name is active
        if(validNextFileName) {
            // adds the next file name buffer contents to the file path
            filePath += nextFileNameBuffer;

            // sets the valid next file name flag as false
            validNextFileName = false;
        }
        else {
            // adds the file name to the file path
            filePath += bufferHeader->name;
        }

        // converts the file path to char pointer
        char *filePathChar = (char *) filePath.c_str();

        // in case the value is not empty
        if(strcmp(bufferHeader->linkFlag, "")) {
            // switches in the file type
            switch(bufferHeader->linkFlag[0]) {
                // in case it's a file
                case '0':
                    // creates a new file stream
                    targetFile = new std::fstream(filePathChar, std::fstream::out | std::fstream::trunc | std::fstream::binary);

                    // in case the size is bigger than zero
                    if(sizeInt > 0) {
                        for(sizeRemaining = sizeInt; sizeRemaining > 0; sizeRemaining -= count) {
                            // in case the end of file has been reached
                            if(file.eof())
                                break;

                            // calculates the read count to be used
                            readCount = sizeRemaining < TAR_BUFFER_SIZE ? sizeRemaining : TAR_BUFFER_SIZE;

                            // reads the file to the buffer
                            file.read(buffer, readCount);

                            // retrieves the number of bytes read
                            count = file.gcount();

                            // writes the file buffer contents
                            targetFile->write(buffer, count);
                        }

                        // calculates the remainder value of the size of the file and the tar buffer base
                        remainderValue = sizeInt % TAR_BUFFER_BASE;

                        // in case the remainder is greater than zero
                        if(remainderValue != 0)
                            file.seekg(TAR_BUFFER_BASE - remainderValue, std::fstream::cur);
                    }

                    // closes the target file
                    targetFile->close();

                    // deletes the target file reference
                    delete targetFile;

                    break;
                // in case it's a directory
                case '5':
                    // creates a new directory
                    _mkdir(filePathChar);

                    break;
                case 'K':
                    // reads the file to the buffer
                    file.read(nextFileNameBuffer, sizeInt);

                    // calculates the remainder value of the size of the file and the tar buffer base
                    remainderValue = sizeInt % TAR_BUFFER_BASE;

                    // in case the remainder is greater than zero
                    if(remainderValue != 0)
                        file.seekg(TAR_BUFFER_BASE - remainderValue, std::fstream::cur);

                    // sets the valid next file name flag
                    validNextFileName = true;
                case 'L':
                    // reads the file to the buffer
                    file.read(nextFileNameBuffer, sizeInt);

                    // calculates the remainder value of the size of the file and the tar buffer base
                    remainderValue = sizeInt % TAR_BUFFER_BASE;

                    // in case the remainder is greater than zero
                    if(remainderValue != 0)
                        file.seekg(TAR_BUFFER_BASE - remainderValue, std::fstream::cur);

                    // sets the valid next file name flag
                    validNextFileName = true;
                default:
                    break;
            }
        }
    }

    // closes the file
    file.close();
}
