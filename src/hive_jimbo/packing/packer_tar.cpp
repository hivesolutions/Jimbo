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

#include "packer_tar.h"

JBPackerTar::JBPackerTar() : JBPacker() {
}

JBPackerTar::~JBPackerTar() {
}

void JBPackerTar::PackFile(std::string &file_path, std::string &target_path) {
}

void JBPackerTar::UnpackFile(std::string &file_path, std::string &target_path) {
    // creates the counter
    int count;

    // creates the read counter
    int read_count;

    // creates space for the size
    unsigned long int size_int;

    // creates space for the size remaining
    unsigned long int size_remaining;

    // creates space for the remainder value
    int remainder_value;

    // allocates space for the header buffer
    char base_buffer[TAR_BUFFER_BASE];

    // allocates space for the buffer
    char buffer[TAR_BUFFER_SIZE];

    // allocates space for the next file name buffer
    char next_file_name_buffer[LARGE_NAME_SIZE];

    // creates the valid next file name flag
    bool valid_next_file_name = false;

    // the target file pointer
    std::fstream *target_file;

    // opens the file
    std::fstream file = std::fstream(file_path.c_str(), std::fstream::in | std::fstream::binary);

    // loops continually
    while(1) {
        // in case the end of file has been reached
        if(file.eof())
            break;

        // reads the header of the tar file
        file.read(base_buffer, TAR_BUFFER_BASE);

        // retrieves the buffer header for the read buffer
        TarHeaderType *buffer_header = (TarHeaderType *) base_buffer;

        // retrieves the file size
        sscanf_s(buffer_header->size, "%lo", &size_int);

        // starts the file path with the target path
        std::string file_path = target_path;

        // in case the flag valis next file name is active
        if(valid_next_file_name) {
            // adds the next file name buffer contents to the file path
            file_path += next_file_name_buffer;

            // sets the valid next file name flag as false
            valid_next_file_name = false;
        }
        else {
            // adds the file name to the file path
            file_path += buffer_header->name;
        }

        // converts the file path to char pointer
        char *file_path_char = (char *) file_path.c_str();

        // in case the value is not empty
        if(strcmp(buffer_header->link_flag, "")) {
            // switches in the file type
            switch(buffer_header->link_flag[0]) {
                // in case it's a file
                case '0':
                    // creates a new file stream
                    target_file = new std::fstream(file_path_char, std::fstream::out | std::fstream::trunc | std::fstream::binary);

                    // in case the size is bigger than zero
                    if(size_int > 0) {
                        for(size_remaining = size_int; size_remaining > 0; size_remaining -= count) {
                            // in case the end of file has been reached
                            if(file.eof())
                                break;

                            // calculates the read count to be used
                            read_count = size_remaining < TAR_BUFFER_SIZE ? size_remaining : TAR_BUFFER_SIZE;

                            // reads the file to the buffer
                            file.read(buffer, read_count);

                            // retrieves the number of bytes read
                            count = file.gcount();

                            // writes the file buffer contents
                            target_file->write(buffer, count);
                        }

                        // calculates the remainder value of the size of the file and the tar buffer base
                        remainder_value = size_int % TAR_BUFFER_BASE;

                        // in case the remainder is greater than zero
                        if(remainder_value != 0)
                            file.seekg(TAR_BUFFER_BASE - remainder_value, std::fstream::cur);
                    }

                    // closes the target file
                    target_file->close();

                    // deletes the target file reference
                    delete target_file;

                    break;
                // in case it's a directory
                case '5':
                    // creates a new directory
                    _mkdir(file_path_char);

                    break;
                case 'K':
                    // reads the file to the buffer
                    file.read(next_file_name_buffer, size_int);

                    // calculates the remainder value of the size of the file and the tar buffer base
                    remainder_value = size_int % TAR_BUFFER_BASE;

                    // in case the remainder is greater than zero
                    if(remainder_value != 0)
                        file.seekg(TAR_BUFFER_BASE - remainder_value, std::fstream::cur);

                    // sets the valid next file name flag
                    valid_next_file_name = true;
                case 'L':
                    // reads the file to the buffer
                    file.read(next_file_name_buffer, size_int);

                    // calculates the remainder value of the size of the file and the tar buffer base
                    remainder_value = size_int % TAR_BUFFER_BASE;

                    // in case the remainder is greater than zero
                    if(remainder_value != 0)
                        file.seekg(TAR_BUFFER_BASE - remainder_value, std::fstream::cur);

                    // sets the valid next file name flag
                    valid_next_file_name = true;
                default:
                    break;
            }
        }
    }

    // closes the file
    file.close();
}
