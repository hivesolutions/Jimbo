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

class JBPackerElement;
class JBPackerFile;
class JBPackerDirectory;

class JBPackerElement {
    private:
        std::string name;
        std::string path;

    public:
        JBPackerElement();
        JBPackerElement(std::string &name, std::string &path);
        ~JBPackerElement();
        virtual void AddFile(std::string &file_path, JBPackerFile *packer_file) {};
        virtual void RemoveFile(std::string &file_path) {};
};

class JBPackerFile : public JBPackerElement {
    private:
        std::fstream *file_stream;
        std::string file_extension;
        std::string mime_type;

    public:
        JBPackerFile();
        JBPackerFile(std::string &name, std::string &path, std::fstream *file_stream, std::string &mime_type = std::string("none/none"));
        ~JBPackerFile();
        void AddFile(std::string &file_path, JBPackerFile *packer_file) override;
        void RemoveFile(std::string &file_path) override;
};

class JBPackerDirectory : public JBPackerElement {
    private:
        std::map<std::string, JBPackerElement *> elements_map;

    public:
        JBPackerDirectory();
        JBPackerDirectory(std::string &name, std::string &path);
        ~JBPackerDirectory();
        void AddFile(std::string &file_path, JBPackerFile *packer_file) override;
        void RemoveFile(std::string &file_path) override;
};
