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

#include "string.h"

std::string &JBString::trim(std::string &stringValue) {
    // allocates space for the index to be used during
    // the left and right trimming operation and the values
    // to be used as markers for the start and end index of
    // the trimming throught sub string
    size_t index;
    size_t start;
    size_t end;

    // retrieves the size of the string to be used
    // during iteration to avoid buffer overlfow
    size_t stringSize = stringValue.size();

    // iterates while the string value is not a valid
    // non space chartacter (left trim)
    for(index = 0; index < stringSize && stringValue[index] <= 32; index++) { }

    // marks the start index resulting from the left trim
    // this value is going to be used for the sub string
    start = index;

    // iterates (reversely) while the string value is not a valid
    // non space chartacter (right trim)
    for(index = stringSize; index > 0 && stringValue[index - 1] <= 32; index--) { }

    // marks the end index resulting from the right trim
    // this value is going to be used for the sub string
    end = index;

    // creates the final string value by creating
    // a sub string of the original value on the detected
    // end and start indexed of the trimming process
    stringValue = stringValue.substr(start, end - start);
    return stringValue;
}

std::string &JBString::ltrim(std::string &stringValue) {
    // allocates space for the index to be used during
    // the left trimming operation and the values to
    // be used as markers for the start and end index of
    // the trimming throught sub string
    size_t index;
    size_t start;
    size_t end;

    // retrieves the size of the string to be used
    // during iteration to avoid buffer overlfow
    size_t stringSize = stringValue.size();

    // iterates while the string value is not a valid
    // non space chartacter (left trim)
    for(index = 0; (stringValue[index] <= 32) && index < stringSize; index++) { }

    // sets start index as the index resulting from the left trim
    // and the end index as the string size (to the end of string)
    start = index;
    end = stringSize;

    // creates the final string value by creating
    // a sub string of the original value on the detected
    // end and start indexed of the trimming process
    stringValue = stringValue.substr(start, end - start);
    return stringValue;
}

std::string &JBString::rtrim(std::string &stringValue) {
    // allocates space for the index to be used during
    // the right trimming operation and the values to
    // be used as markers for the start and end index of
    // the trimming throught sub string
    size_t index;
    size_t start;
    size_t end;

    // retrieves the size of the string to be used
    // during iteration to avoid buffer overlfow
    size_t stringSize = stringValue.size();

    // iterates (reversely) while the string value is not a valid
    // non space chartacter (right trim)
    for(index = stringSize; index > 0 && stringValue[index - 1] <= 32; index--) { }

    // sets the start index as zero (begining of the string)
    // and the end index as the index resulting from the right trim
    start = 0;
    end = index;

    // creates the final string value by creating
    // a sub string of the original value on the detected
    // end and start indexed of the trimming process
    stringValue = stringValue.substr(start, end - start);
    return stringValue;
}
