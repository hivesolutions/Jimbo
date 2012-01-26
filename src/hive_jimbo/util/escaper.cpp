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

#include "escaper.h"

JBEscaper::JBEscaper() {
}

JBEscaper::~JBEscaper() {
}

std::string &JBEscaper::escape(std::string &stringValue) {
    JBEscaper::escapeCharacterSequence(stringValue, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::escapeCharacterSequence(stringValue, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::escapeCharacterSequence(stringValue, ARGUMENT_TYPE_TOKEN);
    return stringValue;
}

std::string *JBEscaper::escape(char *stringCharacterValue) {
    std::string *stringValue = new std::string(stringCharacterValue);
    JBEscaper::escapeCharacterSequence(*stringValue, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::escapeCharacterSequence(*stringValue, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::escapeCharacterSequence(*stringValue, ARGUMENT_TYPE_TOKEN);
    return stringValue;
}

std::string &JBEscaper::unescape(std::string &stringValue) {
    JBEscaper::unescapeCharacterSequence(stringValue, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::unescapeCharacterSequence(stringValue, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::unescapeCharacterSequence(stringValue, ARGUMENT_TYPE_TOKEN);
    return stringValue;
}

std::string *JBEscaper::unescape(char *stringCharacterValue) {
    std::string *stringValue = new std::string(stringCharacterValue);
    JBEscaper::unescapeCharacterSequence(*stringValue, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::unescapeCharacterSequence(*stringValue, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::unescapeCharacterSequence(*stringValue, ARGUMENT_TYPE_TOKEN);
    return stringValue;
}

std::string &JBEscaper::escapeCharacterSequence(std::string &stringValue, const char *characterSequence) {
    std::string characterSequenceEscaped = std::string(ESCAPE_TOKEN) + std::string(characterSequence);

    JBEscaper::replaceAll(stringValue, characterSequence, characterSequenceEscaped);

    // returns the string value
    return stringValue;
}

std::string &JBEscaper::unescapeCharacterSequence(std::string &stringValue, const char *characterSequence) {
    std::string characterSequenceEscaped = std::string(ESCAPE_TOKEN) + std::string(characterSequence);

    JBEscaper::replaceAll(stringValue, characterSequenceEscaped, characterSequence);

    // returns the string value
    return stringValue;
}

std::string &JBEscaper::replaceAll(std::string &targetString, const std::string &originalSequence, const std::string &targetSequence) {
    size_t lookHere = 0;
    size_t foundHere;

    while((foundHere = targetString.find(originalSequence, lookHere)) != std::string::npos) {
        targetString.replace(foundHere, originalSequence.size(), targetSequence);
        lookHere = foundHere + targetSequence.size();
    }

    return targetString;
}

void JBEscaper::releaseStringValue(std::string *stringValue) {
    delete stringValue;
}
