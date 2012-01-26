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

#include "formatter.h"

JBLoggerFormatter::JBLoggerFormatter() {
}

JBLoggerFormatter::~JBLoggerFormatter() {
}

void JBLoggerFormatter::format(JBLoggerRecord &record) {
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

JBLoggerTimeFormatter::JBLoggerTimeFormatter() : JBLoggerFormatter() {
}

JBLoggerTimeFormatter::~JBLoggerTimeFormatter() {
}

void JBLoggerTimeFormatter::format(JBLoggerRecord &record) {
    JBLoggerFormatter::format(record);

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

JBLoggerStringFormatter::JBLoggerStringFormatter() : JBLoggerFormatter() {
}

JBLoggerStringFormatter::JBLoggerStringFormatter(std::string &formatString) : JBLoggerFormatter() {
    this->setFormatString(formatString);
}

JBLoggerStringFormatter::~JBLoggerStringFormatter() {
}

void JBLoggerStringFormatter::setFormatString(std::string &formatString) {
    this->formatString = formatString;
}
