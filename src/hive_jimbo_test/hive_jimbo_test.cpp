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

int _tmain(int argc, _TCHAR* argv[]) {
    JBHttpClient colony_http = JBHttpClient();

    // retrieves the remote contents
    colony_http.GetContents(std::string("http://www.hive.pt"));

    // opens the file for read and write
    std::fstream file = std::fstream("c:/tobias.dump", std::fstream::in | std::fstream::out | std::fstream::trunc | std::fstream::binary);
    file.write(colony_http.GetMessageBuffer(), colony_http.GetMessageSize());
    file.close();

    char * value = "adasd ";
    std::string trim_a = JBString::Trim(std::string(value));
    std::string ltrim_a = JBString::LTrim(std::string(value));
    std::string rtrim_a = JBString::RTrim(std::string(value));

    std::string install_path = JBPython::GetInstallPath(std::string("2.7"));

    JBLogger *logger = JBLogger::GetLogger();

    JBLoggerFileHandler *file_handler = new JBLoggerFileHandler(std::string("default.log"));
    logger->SetLevel(DEBUG);
    logger->AddHandler(file_handler);
    logger->Critical(std::string("Hello World Test"));
    logger->Debug("Hello %d World Test", 44);

    getchar();

    return 0;
}
