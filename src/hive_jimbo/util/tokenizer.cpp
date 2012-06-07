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

#include "tokenizer.h"

int JBIsSpace::operator()(char c) const {
    return isspace(c);
}

bool JBIsComma::operator()(char c) const {
    return (',' == c);
}

bool JBIsFromString::operator()(char c) const {
    int i_find = this->token_string.find(c);
    if(i_find != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

bool JBIsFromStringNoEscape::operator()(char c) {
    if(c == ESCAPE_CHARACTER) {
        if(this->previous_escape) {
            this->previous_escape = false;
        }
        else {
            this->previous_escape = true;
            return false;
        }
    }

    int i_find = token_string.find(c);
    if(i_find != std::string::npos && !this->previous_escape) {
        return true;
    } else {
        this->previous_escape = false;
        return false;
    }
}

void JBTokenizer::Tokenize(std::vector<std::string> &ro_result, std::string const &rostr, JBIsFromString const &ro_t) {
    ro_result.clear();
    std::string::const_iterator it = rostr.begin();
    std::string::const_iterator it_token_end = rostr.begin();

    while(it != rostr.end()) {
        while(ro_t(*it)) {
            it++;
        }

        // finds next token
        it_token_end = find_if(it, rostr.end(), ro_t);

        // in case the iterator is not at the end
        if(it < it_token_end) {
            // appends the token to result
            ro_result.push_back(std::string(it, it_token_end));
        }

        // sets the new iterator position
        it = it_token_end;
  }
}

void JBTokenizer::TokenizeNoEscape(std::vector<std::string> &ro_result, std::string const &rostr, JBIsFromStringNoEscape &ro_t) {
    ro_result.clear();
    std::string::const_iterator it = rostr.begin();
    std::string::const_iterator it_token_end = rostr.begin();

    while(it != rostr.end()) {
        while(ro_t(*it))
            it++;

        // finds next token
        it_token_end = find_if(it, rostr.end(), ro_t);

        // in case the iterator is not at the end
        if(it < it_token_end) {
            // appends the token to result
            ro_result.push_back(std::string(it, it_token_end));
        }

        // sets the new iterator position
        it = it_token_end;
    }
}
