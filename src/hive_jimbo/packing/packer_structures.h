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

class CPackerElement;
class CPackerFile;
class CPackerDirectory;

class CPackerElement {
    private:
        std::string name;
        std::string path;

    public:
        CPackerElement();
        CPackerElement(std::string &name, std::string &path);
        ~CPackerElement();
        virtual void addFile(std::string &filePath, CPackerFile *packerFile) {};
        virtual void removeFile(std::string &filePath) {};
};

class CPackerFile : public CPackerElement {
    private:
        std::fstream *fileStream;
        std::string fileExtension;
        std::string mimeType;

    public:
        CPackerFile();
        CPackerFile(std::string &name, std::string &path, std::fstream *fileStream, std::string &mimeType = std::string("none/none"));
        ~CPackerFile();
        void addFile(std::string &filePath, CPackerFile *packerFile) override;
        void removeFile(std::string &filePath) override;
};

class CPackerDirectory : public CPackerElement {
    private:
        std::map<std::string, CPackerElement *> elementsMap;

    public:
        CPackerDirectory();
        CPackerDirectory(std::string &name, std::string &path);
        ~CPackerDirectory();
        void addFile(std::string &filePath, CPackerFile *packerFile) override;
        void removeFile(std::string &filePath) override;
};
