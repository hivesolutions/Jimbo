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

#include "logger.h"

JBLogger::JBLogger() {
    // creates new instances for default logging and
    // default logger formatter
    JBLoggerHandler *defaultLoggerHandler = new DEFAULT_HANDLER();
    JBLoggerFormatter *defaultLoggerFormatter = new DEFAULT_FORMATTER();

    // sets the default log level that provides initial
    // support for imediate logging
    this->logLevel = WARNING;

    // adds the default logger handler and the default
    // logger formatter, to provide an initial setup for
    // imediate usage
    this->addHandler(defaultLoggerHandler);
    this->setFormatter(defaultLoggerFormatter);
}

JBLogger::~JBLogger() {
}

void JBLogger::debug(char *format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::debug(value);
}

void JBLogger::info(char *format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::info(value);
}

void JBLogger::warning(char *format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::warning(value);
}

void JBLogger::fault(char *format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::fault(value);
}

void JBLogger::critical(char *format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::critical(value);
}

void JBLogger::debug(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, DEBUG);
    this->handle(record);
}

void JBLogger::info(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, INFO);
    this->handle(record);
}

void JBLogger::warning(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, WARNING);
    this->handle(record);
}

void JBLogger::fault(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, FAULT);
    this->handle(record);
}

void JBLogger::critical(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, CRITICAL);
    this->handle(record);
}

void JBLogger::handle(JBLoggerRecord &record) {
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
        JBLoggerHandler *loggerHandler = *iterator;
        loggerHandler->handle(record);
    }
}

void JBLogger::setLevel(unsigned int level) {
    this->logLevel = level;
}

void JBLogger::addHandler(JBLoggerHandler *loggerHandler) {
    this->handlersList.push_back(loggerHandler);
}

void JBLogger::setDefaultHandler(JBLoggerHandler *defaultLoggerHandler) {
}

void JBLogger::setFormatter(JBLoggerFormatter *loggerFormatter) {
    this->loggerFormatter = loggerFormatter;
}

JBLogger *JBLogger::getLogger(std::string &loggerName) {
    JBLogger *logger;
    loggersMapType::iterator iterator = JBLogger::loggersMap.find(loggerName);

    if(iterator != loggersMap.end()) {
        logger = iterator->second;
    } else {
        // creates a new logger and sets it the
        // proper manner in the loggers map
        logger = new JBLogger();
        JBLogger::loggersMap[loggerName] = logger;
    }

    // returns the retrieved logger, this logger
    // may have been created as a new one
    return logger;
}

JBLogger *JBLogger::getLogger(char *loggerName) {
    return JBLogger::getLogger(std::string(loggerName));
}

JBLogger *JBLogger::getLogger() {
    return JBLogger::getLogger(DEFAULT_LOGGER_NAME);
}

loggersMapType JBLogger::loggersMap = loggersMapType();
