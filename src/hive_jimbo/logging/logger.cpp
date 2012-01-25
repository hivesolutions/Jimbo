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

#include "logger.h"

CLogger::CLogger() {
    // creates new instances for default logging and
    // default logger formatter
    CLoggerHandler *defaultLoggerHandler = new DEFAULT_HANDLER();
    CLoggerFormatter *defaultLoggerFormatter = new DEFAULT_FORMATTER();

    // sets the default log level that provides initial
    // support for imediate logging
    this->logLevel = WARNING;

    // adds the default logger handler and the default
    // logger formatter, to provide an initial setup for
    // imediate usage
    this->addHandler(defaultLoggerHandler);
    this->setFormatter(defaultLoggerFormatter);
}

CLogger::~CLogger() {
}

void CLogger::debug(std::string &value) {
    CLoggerRecord record = CLoggerRecord(value, DEBUG);
    this->handle(record);
}

void CLogger::info(std::string &value) {
    CLoggerRecord record = CLoggerRecord(value, INFO);
    this->handle(record);
}

void CLogger::warning(std::string &value) {
    CLoggerRecord record = CLoggerRecord(value, WARNING);
    this->handle(record);
}

void CLogger::fault(std::string &value) {
    CLoggerRecord record = CLoggerRecord(value, FAULT);
    this->handle(record);
}

void CLogger::critical(std::string &value) {
    CLoggerRecord record = CLoggerRecord(value, CRITICAL);
    this->handle(record);
}

void CLogger::handle(CLoggerRecord &record) {
    // in case the log level is not suficient ignores it
    // nothing should be done (returns immediately)
    if(record.getLevel() < this->logLevel) { return; }

    // calls the logger format to format the record, this
    // should change its internal structure
    this->loggerFormatter->format(record);

    // iterates over all the handlers to handle the record
    // in the proper manner (sends it over all the handlers)
    for(handlersListType::iterator iterator = this->handlersList.begin(); iterator != this->handlersList.end(); iterator++) {
        // retrieves the current logger handler and
        // sends the record to the it for handling
        CLoggerHandler *loggerHandler = *iterator;
        loggerHandler->handle(record);
    }
}

void CLogger::setLevel(unsigned int level) {
    this->logLevel = level;
}

void CLogger::addHandler(CLoggerHandler *loggerHandler) {
    this->handlersList.push_back(loggerHandler);
}

void CLogger::setDefaultHandler(CLoggerHandler *defaultLoggerHandler) {
}

void CLogger::setFormatter(CLoggerFormatter *loggerFormatter) {
    this->loggerFormatter = loggerFormatter;
}

CLogger *CLogger::getLogger(std::string &loggerName) {
    CLogger *logger;
    loggersMapType::iterator iterator = CLogger::loggersMap.find(loggerName);

    if(iterator != loggersMap.end()) {
        logger = iterator->second;
    } else {
        // creates a new logger and sets it the
        // proper manner in the loggers map
        logger = new CLogger();
        CLogger::loggersMap[loggerName] = logger;
    }

    // returns the retrieved logger, this logger
    // may have been created as a new one
    return logger;
}

CLogger *CLogger::getLogger(char *loggerName) {
    return CLogger::getLogger(std::string(loggerName));
}

CLogger *CLogger::getLogger() {
    return CLogger::getLogger(DEFAULT_LOGGER_NAME);
}

loggersMapType CLogger::loggersMap = loggersMapType();

CLoggerHandler::CLoggerHandler() {
}

CLoggerHandler::~CLoggerHandler() {
}

CLoggerStreamHandler::CLoggerStreamHandler() : CLoggerHandler() {
    this->streamOpen = false;
}

CLoggerStreamHandler::~CLoggerStreamHandler() {
}

void CLoggerStreamHandler::handle(CLoggerRecord &record) {
    // in case the stream is not currently open,
    // must open it to start writing
    if(this->streamOpen == NULL) { this->openStream(); }

    // retrieves the formatted value of the records writes
    // it to the stream and flushes the stream to provoke
    // the output to be writen to the stream
    *(this->stream) << record.getFormattedValue() << "\n";
    this->flushStream();
}

CLoggerStandardOutHandler::CLoggerStandardOutHandler() : CLoggerStreamHandler() {
}

CLoggerStandardOutHandler::~CLoggerStandardOutHandler() {
}

void CLoggerStandardOutHandler::openStream() {
    CLoggerStreamHandler::openStream();
    this->stream = &std::cout;
}

CLoggerFileHandler::CLoggerFileHandler() : CLoggerStreamHandler() {
}

CLoggerFileHandler::CLoggerFileHandler(std::string &fileName) : CLoggerStreamHandler() {
    this->setFileName(fileName);
    this->openStream();
}

CLoggerFileHandler::~CLoggerFileHandler() {
}

void CLoggerFileHandler::openStream() {
    CLoggerStreamHandler::openStream();
    std::ofstream *stream = new std::ofstream();
    stream->open(this->fileName.c_str(), DEFAULT_FILE_STREAM_OPEN_MODE);
    this->stream = stream;
}

void CLoggerFileHandler::closeStream() {
    CLoggerStreamHandler::closeStream();
    std::ofstream *stream = (std::ofstream *) this->stream;
    stream->close();
}

void CLoggerFileHandler::setFileName(std::string &fileName) {
    this->fileName = fileName;
}

CLoggerFormatter::CLoggerFormatter() {
}


CLoggerFormatter::~CLoggerFormatter() {
}

void CLoggerFormatter::format(CLoggerRecord &record) {
    // retrieves the record level and value to be
    // used in the construction of the formatted value
    unsigned int recordLevel = record.getLevel();
    std::string &recordValue = record.getValue();

    // creates the record level string
    std::string recordLevelString;

    switch(recordLevel) {
        case DEBUG:
            recordLevelString += "<DEBUG>";
            break;
        case INFO:
            recordLevelString += "<INFO>";
            break;
        case WARNING:
            recordLevelString += "<WARNING>";
            break;
        case FAULT:
            recordLevelString += "<FAULT>";
            break;
        case CRITICAL:
            recordLevelString += "<CRITICAL>";
            break;
    }

    // creates the recrod formatted value from the
    // record level string and the record value and
    // sets it in the record (for latter usage)
    std::string recordFormattedValue = recordLevelString + " " + recordValue;
    record.setFormattedValue(recordFormattedValue);
}

CLoggerTimeFormatter::CLoggerTimeFormatter() : CLoggerFormatter() {
}

CLoggerTimeFormatter::~CLoggerTimeFormatter() {
}

void CLoggerTimeFormatter::format(CLoggerRecord &record) {
    CLoggerFormatter::format(record);

    // retrieves the formatted value
    std::string &formattedValue = record.getFormattedValue();

    // creates the time buffer
    char timeBuffer[1024];

    // creates the local time value
    tm localTimeValue;

    // retrieves the current time value
    time_t timeValue = time(NULL);

    // converts the time to local time
    localtime_s(&localTimeValue, &timeValue);

    // formats the time
    strftime(timeBuffer, 1024, "%Y-%m-%d %H:%M:%S", &localTimeValue);

    // creates the time string
    std::string &timeString = std::string(timeBuffer);

    // creates the new formatted value
    std::string &newFormattedValue = std::string(timeString + " " + formattedValue);

    // sets the new formatted value in the record
    record.setFormattedValue(newFormattedValue);
}

CLoggerStringFormatter::CLoggerStringFormatter() : CLoggerFormatter() {
}

CLoggerStringFormatter::CLoggerStringFormatter(std::string &formatString) : CLoggerFormatter() {
    this->setFormatString(formatString);
}

CLoggerStringFormatter::~CLoggerStringFormatter() {
}

void CLoggerStringFormatter::setFormatString(std::string &formatString) {
    this->formatString = formatString;
}

CLoggerRecord::CLoggerRecord() {
}

CLoggerRecord::CLoggerRecord(std::string &value, unsigned int level) {
    this->setValue(value);
    this->setLevel(level);
}

CLoggerRecord::~CLoggerRecord() {
}

std::string &CLoggerRecord::getValue() {
    return this->value;
}

void CLoggerRecord::setValue(std::string &value) {
    this->value = value;
}

unsigned int CLoggerRecord::getLevel() {
    return this->level;
}

void CLoggerRecord::setLevel(unsigned int level) {
    this->level = level;
}

std::string &CLoggerRecord::getFormattedValue() {
    return this->formattedValue;
}

void CLoggerRecord::setFormattedValue(std::string &formattedValue) {
    this->formattedValue = formattedValue;
}
