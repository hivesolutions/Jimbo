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

#include "handler.h"

JBLoggerHandler::JBLoggerHandler() {
}

JBLoggerHandler::~JBLoggerHandler() {
}

JBLoggerStreamHandler::JBLoggerStreamHandler() : JBLoggerHandler() {
    this->stream_open = false;
}

JBLoggerStreamHandler::~JBLoggerStreamHandler() {
}

void JBLoggerStreamHandler::Handle(JBLoggerRecord &record) {
    // in case the stream is not currently open,
    // must open it to start writing
    if(this->stream_open == NULL) { this->OpenStream(); }

    // retrieves the formatted value of the records writes
    // it to the stream and flushes the stream to provoke
    // the output to be writen to the stream
    *(this->stream) << record.GetFormattedValue() << "\n";
    this->FlushStream();
}

JBLoggerStandardOutHandler::JBLoggerStandardOutHandler() : JBLoggerStreamHandler() {
}

JBLoggerStandardOutHandler::~JBLoggerStandardOutHandler() {
}

void JBLoggerStandardOutHandler::OpenStream() {
    JBLoggerStreamHandler::OpenStream();
    this->stream = &std::cout;
}

JBLoggerFileHandler::JBLoggerFileHandler() : JBLoggerStreamHandler() {
}

JBLoggerFileHandler::JBLoggerFileHandler(std::string &file_name) : JBLoggerStreamHandler() {
    this->SetFileName(file_name);
    this->OpenStream();
}

JBLoggerFileHandler::~JBLoggerFileHandler() {
}

void JBLoggerFileHandler::OpenStream() {
    JBLoggerStreamHandler::OpenStream();
    std::ofstream *stream = new std::ofstream();
    stream->open(this->file_name.c_str(), DEFAULT_FILE_STREAM_MODE);
    this->stream = stream;
}

void JBLoggerFileHandler::CloseStream() {
    JBLoggerStreamHandler::CloseStream();
    std::ofstream *stream = (std::ofstream *) this->stream;
    stream->close();
}

void JBLoggerFileHandler::SetFileName(std::string &file_name) {
    this->file_name = file_name;
}
