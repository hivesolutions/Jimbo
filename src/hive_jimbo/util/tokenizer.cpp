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

#include "stdafx.h"

#include "tokenizer.h"

int JBIsSpace::operator()(char c) const {
    return isspace(c);
}

bool JBIsComma::operator()(char c) const {
    return (',' == c);
}

bool JBIsFromString::operator()(char c) const {
    int iFind = this->tokenString.find(c);
    if(iFind != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool JBIsFromStringNoEscape::operator()(char c) {
    if(c == ESCAPE_CHARACTER) {
        if(this->previousEscape) {
            this->previousEscape = false;
        }
        else {
            this->previousEscape = true;
            return false;
        }
    }

    int iFind = tokenString.find(c);
    if(iFind != std::string::npos && !this->previousEscape) {
        return true;
    } else {
        this->previousEscape = false;
        return false;
    }
}

void JBTokenizer::Tokenize(std::vector<std::string> &roResult, std::string const &rostr, JBIsFromString const &roT) {
    roResult.clear();
    std::string::const_iterator it = rostr.begin();
    std::string::const_iterator itTokenEnd = rostr.begin();

    while(it != rostr.end()) {
        while(roT(*it)) {
            it++;
        }

        // finds next token
        itTokenEnd = find_if(it, rostr.end(), roT);

        // in case the iterator is not at the end
        if(it < itTokenEnd) {
            // appends the token to result
            roResult.push_back(std::string(it, itTokenEnd));
        }

        // sets the new iterator position
        it = itTokenEnd;
  }
}

void JBTokenizer::TokenizeNoEscape(std::vector<std::string> &roResult, std::string const &rostr, JBIsFromStringNoEscape &roT) {
    roResult.clear();
    std::string::const_iterator it = rostr.begin();
    std::string::const_iterator itTokenEnd = rostr.begin();

    while(it != rostr.end()) {
        while(roT(*it))
            it++;

        // finds next token
        itTokenEnd = find_if(it, rostr.end(), roT);

        // in case the iterator is not at the end
        if(it < itTokenEnd) {
            // appends the token to result
            roResult.push_back(std::string(it, itTokenEnd));
        }

        // sets the new iterator position
        it = itTokenEnd;
    }
}
