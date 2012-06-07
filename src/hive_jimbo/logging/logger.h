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

#include "handler.h"
#include "formatter.h"

#define DEFAULT_HANDLER JBLoggerStandardOutHandler
#define DEFAULT_FORMATTER JBLoggerTimeFormatter

#define DEFAULT_FILE_STREAM_MODE std::ios_base::app

#define DEFAULT_LOGGER_NAME "default"

#define STRING_BUFFER_SIZE 1024

#define sprintf_vargs(value, Format, size) va_list vargs;\
    va_start(vargs, Format);\
    vsprintf_s(_value, 1024, Format, vargs);\
    va_end(vargs)

class JBLogger;
class JBLoggerHandler;
class JBLoggerFormatter;
class JBLoggerRecord;

typedef std::vector<JBLoggerHandler *> HandlersListType;
typedef std::map<std::string, JBLogger *> LoggersMapType;

class JBLogger {
    public:
        JB_EXPORT JBLogger();
        JB_EXPORT ~JBLogger();
        JB_EXPORT void Debug(char *Format, ...);
        JB_EXPORT void Info(char *Format, ...);
        JB_EXPORT void Warning(char *Format, ...);
        JB_EXPORT void Fault(char *Format, ...);
        JB_EXPORT void Critical(char *Format, ...);
        JB_EXPORT void Debug(std::string &value);
        JB_EXPORT void Info(std::string &value);
        JB_EXPORT void Warning(std::string &value);
        JB_EXPORT void Fault(std::string &value);
        JB_EXPORT void Critical(std::string &value);
        JB_EXPORT void Handle(JBLoggerRecord &record);
        JB_EXPORT void SetLevel(unsigned int level);
        JB_EXPORT void AddHandler(JBLoggerHandler *logger_handler);
        JB_EXPORT void SetDefaultHandler(JBLoggerHandler *default_logger_handler);
        JB_EXPORT void SetFormatter(JBLoggerFormatter *logger_formatter);
        JB_EXPORT static JBLogger *GetLogger(std::string &logger_name);
        JB_EXPORT static JBLogger *GetLogger(char *logger_name);
        JB_EXPORT static JBLogger *GetLogger();
    private:
        unsigned int log_level;
        JBLoggerHandler *default_logger_handler;
        HandlersListType handlers_list;
        JBLoggerFormatter *logger_formatter;
        static LoggersMapType loggers_map;
};

enum JBLogLevel {
    NOTSET = 0,
    DEBUG,
    INFO,
    WARNING,
    FAULT,
    CRITICAL
};
