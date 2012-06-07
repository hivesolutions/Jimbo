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
    JBLoggerHandler *default_logger_handler = new DEFAULT_HANDLER();
    JBLoggerFormatter *default_logger_formatter = new DEFAULT_FORMATTER();

    // sets the default log level that provides initial
    // support for imediate logging
    this->log_level = WARNING;

    // adds the default logger handler and the default
    // logger formatter, to provide an initial setup for
    // imediate usage
    this->AddHandler(default_logger_handler);
    this->SetFormatter(default_logger_formatter);
}

JBLogger::~JBLogger() {
}

void JBLogger::Debug(char *Format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, Format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::Debug(value);
}

void JBLogger::Info(char *Format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, Format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::Info(value);
}

void JBLogger::Warning(char *Format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, Format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::Warning(value);
}

void JBLogger::Fault(char *Format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, Format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::Fault(value);
}

void JBLogger::Critical(char *Format, ...) {
    char _value[STRING_BUFFER_SIZE];
    sprintf_vargs(_value, Format, STRING_BUFFER_SIZE);
    std::string value = std::string(_value);
    JBLogger::Critical(value);
}

void JBLogger::Debug(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, DEBUG);
    this->Handle(record);
}

void JBLogger::Info(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, INFO);
    this->Handle(record);
}

void JBLogger::Warning(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, WARNING);
    this->Handle(record);
}

void JBLogger::Fault(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, FAULT);
    this->Handle(record);
}

void JBLogger::Critical(std::string &value) {
    JBLoggerRecord record = JBLoggerRecord(value, CRITICAL);
    this->Handle(record);
}

void JBLogger::Handle(JBLoggerRecord &record) {
    // in case the log level is not suficient ignores it
    // nothing should be done (returns immediately)
    if(record.GetLevel() < this->log_level) { return; }

    // calls the logger Format to Format the record, this
    // should change its internal structure
    this->logger_formatter->Format(record);

    // iterates over all the handlers to Handle the record
    // in the proper manner (sends it over all the handlers)
    for(HandlersListType::iterator iterator = this->handlers_list.begin(); iterator != this->handlers_list.end(); iterator++) {
        // retrieves the current logger handler and
        // sends the record to the it for handling
        JBLoggerHandler *logger_handler = *iterator;
        logger_handler->Handle(record);
    }
}

void JBLogger::SetLevel(unsigned int level) {
    this->log_level = level;
}

void JBLogger::AddHandler(JBLoggerHandler *logger_handler) {
    this->handlers_list.push_back(logger_handler);
}

void JBLogger::SetDefaultHandler(JBLoggerHandler *default_logger_handler) {
}

void JBLogger::SetFormatter(JBLoggerFormatter *logger_formatter) {
    this->logger_formatter = logger_formatter;
}

JBLogger *JBLogger::GetLogger(std::string &logger_name) {
    JBLogger *logger;
    LoggersMapType::iterator iterator = JBLogger::loggers_map.find(logger_name);

    if(iterator != loggers_map.end()) {
        logger = iterator->second;
    } else {
        // creates a new logger and sets it the
        // proper manner in the loggers map
        logger = new JBLogger();
        JBLogger::loggers_map[logger_name] = logger;
    }

    // returns the retrieved logger, this logger
    // may have been created as a new one
    return logger;
}

JBLogger *JBLogger::GetLogger(char *logger_name) {
    return JBLogger::GetLogger(std::string(logger_name));
}

JBLogger *JBLogger::GetLogger() {
    return JBLogger::GetLogger(DEFAULT_LOGGER_NAME);
}

LoggersMapType JBLogger::loggers_map = LoggersMapType();
