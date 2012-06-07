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

std::string &JBEscaper::Escape(std::string &string_value) {
    JBEscaper::EscapeCharacterSequence(string_value, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::EscapeCharacterSequence(string_value, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::EscapeCharacterSequence(string_value, ARGUMENT_TYPE_TOKEN);
    return string_value;
}

std::string *JBEscaper::Escape(char *string_character_value) {
    std::string *string_value = new std::string(string_character_value);
    JBEscaper::EscapeCharacterSequence(*string_value, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::EscapeCharacterSequence(*string_value, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::EscapeCharacterSequence(*string_value, ARGUMENT_TYPE_TOKEN);
    return string_value;
}

std::string &JBEscaper::Unescape(std::string &string_value) {
    JBEscaper::UnescapeCharacterSequence(string_value, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::UnescapeCharacterSequence(string_value, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::UnescapeCharacterSequence(string_value, ARGUMENT_TYPE_TOKEN);
    return string_value;
}

std::string *JBEscaper::Unescape(char *string_character_value) {
    std::string *string_value = new std::string(string_character_value);
    JBEscaper::UnescapeCharacterSequence(*string_value, COMMAND_SEPARATOR_TOKEN);
    JBEscaper::UnescapeCharacterSequence(*string_value, ARGUMENT_SEPARATOR_TOKEN);
    JBEscaper::UnescapeCharacterSequence(*string_value, ARGUMENT_TYPE_TOKEN);
    return string_value;
}

std::string &JBEscaper::EscapeCharacterSequence(std::string &string_value, const char *character_sequence) {
    std::string character_sequence_escaped = std::string(ESCAPE_TOKEN) + std::string(character_sequence);

    JBEscaper::ReplaceAll(string_value, character_sequence, character_sequence_escaped);

    // returns the string value
    return string_value;
}

std::string &JBEscaper::UnescapeCharacterSequence(std::string &string_value, const char *character_sequence) {
    std::string character_sequence_escaped = std::string(ESCAPE_TOKEN) + std::string(character_sequence);

    JBEscaper::ReplaceAll(string_value, character_sequence_escaped, character_sequence);

    // returns the string value
    return string_value;
}

std::string &JBEscaper::ReplaceAll(std::string &target_string, const std::string &original_sequence, const std::string &target_sequence) {
    size_t look_here = 0;
    size_t found_here;

    while((found_here = target_string.find(original_sequence, look_here)) != std::string::npos) {
        target_string.replace(found_here, original_sequence.size(), target_sequence);
        look_here = found_here + target_sequence.size();
    }

    return target_string;
}

void JBEscaper::ReleaseStringValue(std::string *string_value) {
    delete string_value;
}
