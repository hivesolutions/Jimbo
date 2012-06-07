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

#include "packer_structures.h"

JBPackerElement::JBPackerElement() {
}

JBPackerElement::JBPackerElement(std::string &name, std::string &path) {
    this->name = name;
    this->path = path;
}

JBPackerElement::~JBPackerElement() {
}

JBPackerFile::JBPackerFile() : JBPackerElement() {
}

JBPackerFile::JBPackerFile(std::string &name, std::string &path, std::fstream *file_stream, std::string &mime_type) : JBPackerElement(name, path) {
    this->file_stream = file_stream;
    this->mime_type = mime_type;
}

JBPackerFile::~JBPackerFile() {
}

void JBPackerFile::AddFile(std::string &file_path, JBPackerFile *packer_file) {
    JBPackerElement::AddFile(file_path, packer_file);

}

void JBPackerFile::RemoveFile(std::string &file_path) {
    JBPackerElement::RemoveFile(file_path);
}

JBPackerDirectory::JBPackerDirectory() {
}

JBPackerDirectory::JBPackerDirectory(std::string &name, std::string &path) : JBPackerElement(name, path) {
}

JBPackerDirectory::~JBPackerDirectory() {
}

void JBPackerDirectory::AddFile(std::string &file_path, JBPackerFile *packer_file) {
    JBPackerElement::AddFile(file_path, packer_file);

    int index = file_path.find("/");

    if(index == std::string::npos) {
        this->elements_map[file_path] = packer_file;
    }
    else {
        std::string file_directory_name = file_path.substr(0, index);
        std::string file_new_path = file_path.substr(index + 1);

        if(this->elements_map.find(file_directory_name) == this->elements_map.end())
            this->elements_map[file_directory_name] = new JBPackerDirectory(file_directory_name, file_directory_name);

         this->elements_map[file_directory_name]->AddFile(file_new_path, packer_file);
    }
}

void JBPackerDirectory::RemoveFile(std::string &file_path) {
    JBPackerElement::RemoveFile(file_path);
}
