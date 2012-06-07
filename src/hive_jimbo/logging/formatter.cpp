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

void JBLoggerFormatter::Format(JBLoggerRecord &record) {
    // retrieves the record level and value to be
    // used in the construction of the formatted value
    unsigned int record_level = record.GetLevel();
    std::string &record_value = record.GetValue();

    // creates the record level string
    std::string record_level_string;

    switch(record_level) {
        case DEBUG:
            record_level_string += "<DEBUG>";
            break;
        case INFO:
            record_level_string += "<INFO>";
            break;
        case WARNING:
            record_level_string += "<WARNING>";
            break;
        case FAULT:
            record_level_string += "<FAULT>";
            break;
        case CRITICAL:
            record_level_string += "<CRITICAL>";
            break;
    }

    // creates the recrod formatted value from the
    // record level string and the record value and
    // sets it in the record (for latter usage)
    std::string record_formatted_value = record_level_string + " " + record_value;
    record.SetFormattedValue(record_formatted_value);
}

JBLoggerTimeFormatter::JBLoggerTimeFormatter() : JBLoggerFormatter() {
}

JBLoggerTimeFormatter::~JBLoggerTimeFormatter() {
}

void JBLoggerTimeFormatter::Format(JBLoggerRecord &record) {
    JBLoggerFormatter::Format(record);

    // retrieves the formatted value
    std::string &formatted_value = record.GetFormattedValue();

    // creates the time buffer
    char time_buffer[1024];

    // creates the local time value
    tm local_time_value;

    // retrieves the current time value
    time_t time_value = time(NULL);

    // converts the time to local time
    localtime_s(&local_time_value, &time_value);

    // formats the time
    strftime(time_buffer, 1024, "%Y-%m-%d %H:%M:%S", &local_time_value);

    // creates the time string
    std::string &time_string = std::string(time_buffer);

    // creates the new formatted value
    std::string &new_formatted_value = std::string(time_string + " " + formatted_value);

    // sets the new formatted value in the record
    record.SetFormattedValue(new_formatted_value);
}

JBLoggerStringFormatter::JBLoggerStringFormatter() : JBLoggerFormatter() {
}

JBLoggerStringFormatter::JBLoggerStringFormatter(std::string &format_string) : JBLoggerFormatter() {
    this->SetFormatString(format_string);
}

JBLoggerStringFormatter::~JBLoggerStringFormatter() {
}

void JBLoggerStringFormatter::SetFormatString(std::string &format_string) {
    this->format_string = format_string;
}
