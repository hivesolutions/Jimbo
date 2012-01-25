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

HKEY JBPython::checkPython() {
    return JBPython::checkPython(NULL);
}

HKEY JBPython::checkPython(const char *version) {
    // allocates space for the string to hold the
    // various sub key values (iteration cycle)
    // stores also the syze of that key in a variable
    char subKeyName[JBPython::bufferLength];
    DWORD subKeyNameSize = JBPython::bufferLength;

    // allocates space for the main python key registry
    // (key) reference, going to be used during the
    // enumeration of the sub keys
    HKEY pythonKey;

    // checks the registry for the python installation value
    if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, PYTHON_KEY_NAME, 0, KEY_READ, &pythonKey) != ERROR_SUCCESS) {
        throw "Python is not installed in the system";
    }

    // iterates continuously, to iterate over all the sub keys
    // in the current registry key, the iteration should only stop
    // in case no more key value are found (exception thrown)
    for(int index = 0; ; index++) {
        // enumerates one more value in the python key value to check
        // it for the correct version (the python sub keys represent
        // the various versions present in the system)
        if(RegEnumKeyEx(pythonKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            // closes the python key, avoids memory leaking
            RegCloseKey(pythonKey);

            throw "No valid Python version installed in the system";
        }

        // in case no version for checking is specified, returns
        // the python key immediately (no version is required)
        if(version == NULL) { return pythonKey; }

        // in case the requested version matches the name of the current
        // sub key, the required python version is found
        if(!strcmp(subKeyName, version)) { return pythonKey; }
    }

    // closes the python key, avoids memory leaking
    RegCloseKey(pythonKey);

    // retuns an invalid result, an error must have ocured
    // not possible to reach end of function otherwise
    return NULL;
}

void JBPython::getAvailableVersions(std::vector<std::string> &versions) {
    // allocates space for the string to hold the
    // various sub key values (iteration cycle)
    // stores also the syze of that key in a variable
    char subKeyName[JBPython::bufferLength];
    DWORD subKeyNameSize = JBPython::bufferLength;

    // retrieves the python (base) key, in case none
    // is found this should raise an exception
    HKEY pythonKey = checkPython();

    // iterates continuously, to iterate over all the sub keys
    // in the current registry key, the iteration should only stop
    // in case no more key value are found (returns function)
    for(int index = 0; ; index++) {
        // enumerates one more value in the python key value to check
        // it for the correct version (the python sub keys represent
        // the various versions present in the system)
        if(RegEnumKeyEx(pythonKey, index, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
            // closes the python key, avoiding leaking
            // and returns the function immediately
            RegCloseKey(pythonKey);
            return;
        }

        // adds the sub key name into the versions list,
        // first convert it into a standard string object
        versions.push_back(std::string(subKeyName));
    }

    // closes the python key, avoids possible leaking
    RegCloseKey(pythonKey);
}

std::string JBPython::getInstallPath(std::string &version) {
    // allocates space for the string to hold the
    // install path, and also stores the size of
    // the path in it's own variable
    char installPath[JBPython::bufferLength];
    DWORD installPathSize = JBPython::bufferLength;

    // retrieves the python (base) key, in case
    // none is found this should raise an exception
    HKEY pythonKey = checkPython();

    // allocates space for the storage of the install
    // path key value
    HKEY installPathKey;

    // tris to open the request version's python install path
    // key value, this should contain the path as a string value
    if(RegOpenKeyEx(pythonKey, (version + "\\InstallPath").c_str(), 0, KEY_READ, &installPathKey) != ERROR_SUCCESS) {
        // closes the python key, avoids leaking
        RegCloseKey(pythonKey);

        throw "The requested version of Python is not installed in the system";
    }

    // tries to obtain the value of the of the install path key into
    // (this should not fail in normal circumstances)
    if(RegQueryValueEx(installPathKey, NULL, NULL, NULL, (LPBYTE) installPath, &installPathSize) != ERROR_SUCCESS) {
         // closes the install path key, avoids leaking
        RegCloseKey(installPathKey);

        throw "Error retrieveing install path value";
    }

    // closes both python key and the install
    // path key values, avoids possible leaking
    RegCloseKey(pythonKey);
    RegCloseKey(installPathKey);

    // returns the string representing the install
    // path (it is returned by value)
    return installPath;
}
