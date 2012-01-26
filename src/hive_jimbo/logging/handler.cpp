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

#include "handler.h"

JBLoggerHandler::JBLoggerHandler() {
}

JBLoggerHandler::~JBLoggerHandler() {
}

JBLoggerStreamHandler::JBLoggerStreamHandler() : JBLoggerHandler() {
    this->streamOpen = false;
}

JBLoggerStreamHandler::~JBLoggerStreamHandler() {
}

void JBLoggerStreamHandler::handle(JBLoggerRecord &record) {
    // in case the stream is not currently open,
    // must open it to start writing
    if(this->streamOpen == NULL) { this->openStream(); }

    // retrieves the formatted value of the records writes
    // it to the stream and flushes the stream to provoke
    // the output to be writen to the stream
    *(this->stream) << record.getFormattedValue() << "\n";
    this->flushStream();
}

JBLoggerStandardOutHandler::JBLoggerStandardOutHandler() : JBLoggerStreamHandler() {
}

JBLoggerStandardOutHandler::~JBLoggerStandardOutHandler() {
}

void JBLoggerStandardOutHandler::openStream() {
    JBLoggerStreamHandler::openStream();
    this->stream = &std::cout;
}

JBLoggerFileHandler::JBLoggerFileHandler() : JBLoggerStreamHandler() {
}

JBLoggerFileHandler::JBLoggerFileHandler(std::string &fileName) : JBLoggerStreamHandler() {
    this->setFileName(fileName);
    this->openStream();
}

JBLoggerFileHandler::~JBLoggerFileHandler() {
}

void JBLoggerFileHandler::openStream() {
    JBLoggerStreamHandler::openStream();
    std::ofstream *stream = new std::ofstream();
    stream->open(this->fileName.c_str(), DEFAULT_FILE_STREAM_MODE);
    this->stream = stream;
}

void JBLoggerFileHandler::closeStream() {
    JBLoggerStreamHandler::closeStream();
    std::ofstream *stream = (std::ofstream *) this->stream;
    stream->close();
}

void JBLoggerFileHandler::setFileName(std::string &fileName) {
    this->fileName = fileName;
}
