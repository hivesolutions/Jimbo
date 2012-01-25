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

#include "python.h"

HKEY CPython::checkPython() {
    char subKeyName[CPython::bufferLength];
    DWORD subKeyNameSize;
    HKEY pythonKeyValue;

    // checks the registry for the python installation value
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Python\\PythonCore", 0, KEY_READ, &pythonKeyValue) != ERROR_SUCCESS)
        throw "Python is not installed in the system";

    for(int index = 0; ; index++) {
        subKeyNameSize = CPython::bufferLength;

        if(RegEnumKeyEx(pythonKeyValue, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            // closes the python key value
            RegCloseKey(pythonKeyValue);

            throw "No valid Python version installed in the system";
        }

        if(!strcmp(subKeyName, "2.5") || !strcmp(subKeyName, "2.6"))
            return pythonKeyValue;
    }

    return NULL;
}

void CPython::getAvailablePythonVersions(std::vector<std::string> &pythonVersionsList) {
    char subKeyName[CPython::bufferLength];
    DWORD subKeyNameSize;
    HKEY pythonKeyValue = checkPython();

    for(int index = 0; ; index++) {
        subKeyNameSize = CPython::bufferLength;

        if(RegEnumKeyEx(pythonKeyValue, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            // closes the python key value
            RegCloseKey(pythonKeyValue);

            return;
        }

        pythonVersionsList.push_back(std::string(subKeyName));
    }
}

std::string CPython::getPythonInstallPath(std::string &pythonVersion) {
    char installPathValue[CPython::bufferLength];
    DWORD installPathValueSize = CPython::bufferLength;
    HKEY pythonKeyValue = checkPython();
    HKEY pythonInstallPathKeyValue;

    if(RegOpenKeyEx(pythonKeyValue, (pythonVersion + "\\InstallPath").c_str(), 0, KEY_READ, &pythonInstallPathKeyValue) != ERROR_SUCCESS) {
        // closes the python key value
        RegCloseKey(pythonKeyValue);

        throw "The requested version of Python is not installed in the system";
    }

    if(RegQueryValueEx(pythonInstallPathKeyValue, NULL, NULL, NULL, (LPBYTE) installPathValue, &installPathValueSize) != ERROR_SUCCESS)
        throw "Error retrieveing install path value";

    return installPathValue;
}
