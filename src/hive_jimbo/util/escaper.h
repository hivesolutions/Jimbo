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

#define ESCAPE_TOKEN "\\"

#define COMMAND_SEPARATOR_TOKEN ";"
#define ARGUMENT_SEPARATOR_TOKEN "&"
#define ARGUMENT_TYPE_TOKEN "=///="

class JBEscaper {
    public:
        JBEscaper();
        ~JBEscaper();
        static std::string &escape(std::string &stringValue);
        static std::string *escape(char *stringCharacterValue);
        static std::string &unescape(std::string &stringValue);
        static std::string *unescape(char *stringCharacterValue);
        static std::string &escapeCharacterSequence(std::string &stringValue, const char *characterSequence);
        static std::string &unescapeCharacterSequence(std::string &stringValue, const char *characterSequence);
        static std::string &replaceAll(std::string &targetString, const std::string &originalSequence, const std::string &targetSequence);
        static void releaseStringValue(std::string *stringValue);
};
