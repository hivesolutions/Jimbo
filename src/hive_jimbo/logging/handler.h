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

#include "logger.h"
#include "record.h"

class JBLoggerHandler;
class JBLoggerStreamHandler;
class JBLoggerStandardOutHandler;
class JBLoggerFileHandler;

class JBLoggerHandler {
    public:
        JB_EXPORT JBLoggerHandler();
        JB_EXPORT ~JBLoggerHandler();
        JB_EXPORT virtual void Handle(JBLoggerRecord &record) {};
        JB_EXPORT void Flush();
};

class JBLoggerStreamHandler : public JBLoggerHandler {
    public:
        JB_EXPORT JBLoggerStreamHandler();
        JB_EXPORT ~JBLoggerStreamHandler();
        JB_EXPORT void Handle(JBLoggerRecord &record);
        JB_EXPORT void Flush();
        JB_EXPORT virtual void OpenStream() { this->stream_open = true; };
        JB_EXPORT virtual void CloseStream() { this->stream_open = false; };
        JB_EXPORT virtual void FlushStream() { this->stream->flush(); };
    protected:
        std::ostream *stream;
        bool stream_open;
};

class JBLoggerStandardOutHandler : public JBLoggerStreamHandler {
    public:
        JB_EXPORT JBLoggerStandardOutHandler();
        JB_EXPORT ~JBLoggerStandardOutHandler();
        JB_EXPORT void Flush();
        JB_EXPORT void OpenStream();
};

class JBLoggerFileHandler : public JBLoggerStreamHandler {
    public:
        JB_EXPORT JBLoggerFileHandler();
        JB_EXPORT JBLoggerFileHandler(std::string &file_name);
        JB_EXPORT ~JBLoggerFileHandler();
        JB_EXPORT void Flush();
        JB_EXPORT void OpenStream();
        JB_EXPORT void CloseStream();
        JB_EXPORT void SetFileName(std::string &file_name);
    private:
        std::string file_name;
        std::ofstream file_stream;
};
