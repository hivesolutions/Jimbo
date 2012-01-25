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

#define DEFAULT_HANDLER CLoggerStandardOutHandler
#define DEFAULT_FORMATTER CLoggerTimeFormatter

#define NOTSET 0
#define DEBUG 1
#define INFO 2
#define WARNING 3
#define FAULT 4
#define CRITICAL 5

#define DEFAULT_FILE_STREAM_OPEN_MODE std::ios_base::app

#define DEFAULT_LOGGER_NAME "default"

class CLogger;
class CLoggerHandler;
class CLoggerStreamHandler;
class CLoggerStandardOutHandler;
class CLoggerFileHandler;
class CLoggerFormatter;
class CLoggerTimeFormatter;
class CLoggerStringFormatter;
class CLoggerRecord;

typedef std::vector<CLoggerHandler *> handlersListType;
typedef std::map<std::string, CLogger *> loggersMapType;

class CLogger {
    public:
        CLogger();
        ~CLogger();
        void debug(std::string &value);
        void info(std::string &value);
        void warning(std::string &value);
        void fault(std::string &value);
        void critical(std::string &value);
        void handle(CLoggerRecord &record);
        void setLevel(unsigned int level);
        void addHandler(CLoggerHandler *loggerHandler);
        void setDefaultHandler(CLoggerHandler *defaultLoggerHandler);
        void setFormatter(CLoggerFormatter *loggerFormatter);
        static CLogger *getLogger(std::string &loggerName);
        static CLogger *getLogger(char *loggerName);
        static CLogger *getLogger();
    private:
        unsigned int logLevel;
        CLoggerHandler *defaultLoggerHandler;
        handlersListType handlersList;
        CLoggerFormatter *loggerFormatter;
        static loggersMapType loggersMap;
};

class CLoggerHandler {
    public:
        CLoggerHandler();
        ~CLoggerHandler();
        virtual void handle(CLoggerRecord &record) {};
        void flush();
};

class CLoggerStreamHandler : public CLoggerHandler {
    public:
        CLoggerStreamHandler();
        ~CLoggerStreamHandler();
        void handle(CLoggerRecord &record);
        void flush();
        virtual void openStream() { this->streamOpen = true; };
        virtual void closeStream() { this->streamOpen = false; };
        virtual void flushStream() { this->stream->flush(); };
    protected:
        std::ostream *stream;
        bool streamOpen;
};

class CLoggerStandardOutHandler : public CLoggerStreamHandler {
    public:
        CLoggerStandardOutHandler();
        ~CLoggerStandardOutHandler();
        void flush();
        void openStream();
};

class CLoggerFileHandler : public CLoggerStreamHandler {
    public:
        CLoggerFileHandler();
        CLoggerFileHandler(std::string &fileName);
        ~CLoggerFileHandler();
        void flush();
        void openStream();
        void closeStream();
        void setFileName(std::string &fileName);
    private:
        std::string fileName;
        std::ofstream fileStream;
};

class CLoggerFormatter {
    public:
        CLoggerFormatter();
        ~CLoggerFormatter();
        virtual void format(CLoggerRecord &record);
};

class CLoggerTimeFormatter : public CLoggerFormatter {
    public:
        CLoggerTimeFormatter();
        ~CLoggerTimeFormatter();
        void format(CLoggerRecord &record);
        void formatValue(std::string &value);
};

class CLoggerStringFormatter : public CLoggerFormatter {
    public:
        CLoggerStringFormatter();
        CLoggerStringFormatter(std::string &formatString);
        ~CLoggerStringFormatter();
        void setFormatString(std::string &formatString);
    private:
        std::string formatString;
};

class CLoggerRecord {
    public:
        CLoggerRecord();
        CLoggerRecord(std::string &value, unsigned int level);
        ~CLoggerRecord();
        std::string &getValue();
        void setValue(std::string &value);
        unsigned int getLevel();
        void setLevel(unsigned int level);
        std::string &getFormattedValue();
        void setFormattedValue(std::string &formattedValue);
    private:
        std::string value;
        unsigned int level;
        std::string formattedValue;
};
