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

#include "string.h"

std::string &CString::trim(std::string &stringValue) {
    int i;
    int j;
    int start;
    int end;

    for(i = 0; (stringValue[i] != 0 && stringValue[i] <= 32);) {
        i++;
    }

    start = i;

    for(i = 0, j = 0; stringValue[i] != 0; i++) {
        j = ((stringValue[i] <= 32) ? j + 1 : 0);
    }

    // calculates the end trimming index as the difference
    // between the start index (left trim) and the final
    /// (right trim) indexes
    end = i - j;

    // creates the final string value by creating
    // a sub string of the original value on the detected
    // end and start indexed of the trimming process
    stringValue = stringValue.substr(start, end - start);
    return stringValue;
}
