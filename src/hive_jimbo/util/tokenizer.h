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

/**
 * The character to be used as the proper
 * esacape character.
 */
#define ESCAPE_CHARACTER '\\'

class JBIsSpace : public std::unary_function<char, bool> {
    public:
        int operator()(char c) const;
};

class JBIsComma : public std::unary_function<char, bool> {
    public:
        bool operator()(char c) const;
};

class JBIsFromString : public std::unary_function<char, bool> {
    public:
        JBIsFromString::JBIsFromString(std::string const &token_string) : token_string(token_string) {}
        bool operator()(char c) const;
    protected:
        std::string token_string;
};

class JBIsFromStringNoEscape : JBIsFromString {
    public:
        JBIsFromStringNoEscape::JBIsFromStringNoEscape(std::string const &token_string) : JBIsFromString(token_string) {
            this->previous_escape = false;
        }
        bool operator()(char c);
    private:
        bool previous_escape;
};

class JBTokenizer {
    public:
        static void Tokenize(std::vector<std::string> &ro_result, std::string const &rostr, JBIsFromString const &ro_t);
        static void TokenizeNoEscape(std::vector<std::string> &ro_result, std::string const &rostr, JBIsFromStringNoEscape &ro_t);
};
