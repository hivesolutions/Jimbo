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

#include "logger.h"
#include "record.h"

class JBLoggerHandler;
class JBLoggerStreamHandler;
class JBLoggerStandardOutHandler;
class JBLoggerFileHandler;

class JBLoggerHandler {
    public:
        JBLoggerHandler();
        ~JBLoggerHandler();
        virtual void handle(JBLoggerRecord &record) {};
        void flush();
};

class JBLoggerStreamHandler : public JBLoggerHandler {
    public:
        JBLoggerStreamHandler();
        ~JBLoggerStreamHandler();
        void handle(JBLoggerRecord &record);
        void flush();
        virtual void openStream() { this->streamOpen = true; };
        virtual void closeStream() { this->streamOpen = false; };
        virtual void flushStream() { this->stream->flush(); };
    protected:
        std::ostream *stream;
        bool streamOpen;
};

class JBLoggerStandardOutHandler : public JBLoggerStreamHandler {
    public:
        JBLoggerStandardOutHandler();
        ~JBLoggerStandardOutHandler();
        void flush();
        void openStream();
};

class JBLoggerFileHandler : public JBLoggerStreamHandler {
    public:
        JBLoggerFileHandler();
        JBLoggerFileHandler(std::string &fileName);
        ~JBLoggerFileHandler();
        void flush();
        void openStream();
        void closeStream();
        void setFileName(std::string &fileName);
    private:
        std::string fileName;
        std::ofstream fileStream;
};
