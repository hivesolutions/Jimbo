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

#include "process.h"

bool JBProcess::isRunning(char *exeName, bool *running) {
    // in case the boolean is already false, should
    // return immediately
    if(!running) { return false; }

    // the process entry structure
    PROCESSENTRY32 processEntry;

    // flag for the function success
    bool success = true;

    // sets the process running flag to false, by default
    *running = false;

    // takes a snapshot of the specified processes,
    // as well as the heaps, modules, and threads used by these processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if(hSnapshot != INVALID_HANDLE_VALUE) {
        // sets the size of the structure before using it
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        // describes an entry from a list of the processes residing
        // in the system address space when a snapshot was taken
        BOOL processRetrievalResult = Process32First(hSnapshot, &processEntry);

        // iterates ovell all the processes residing
        // in the system address
        while(processRetrievalResult == TRUE) {
            // in case the process found is the requested
            if (!strcmp(exeName, processEntry.szExeFile)) {
                *running = true;
                break;
            }

            // retrieves information about the next
            // process recorded in a system snapshot
            processRetrievalResult = Process32Next(hSnapshot, &processEntry);
        }

        // in case there was an error or there are no more
        // files to be retrieved
        if (processRetrievalResult != TRUE && GetLastError() != ERROR_NO_MORE_FILES)
            success = false;

        // closes the snapshot handle
        CloseHandle(hSnapshot);
    }
    else {
        success = false;
    }

    return success;
}

bool JBProcess::getRunning(char *exeName, int *count) {
    // the process entry structure
    PROCESSENTRY32 processEntry;

    // flag for the function success
    bool success = true;

    // sets the intial number of processes running (count variable)
    *count = 0;

    // takes a snapshot of the specified processes,
    // as well as the heaps, modules, and threads used by these processes
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if(hSnapshot != INVALID_HANDLE_VALUE) {
        // sets the size of the structure before using it
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        // describes an entry from a list of the processes residing
        // in the system address space when a snapshot was taken
        BOOL processRetrievalResult = Process32First(hSnapshot, &processEntry);

        // iterates ovell all the processes residing
        // in the system address
        while(processRetrievalResult == TRUE) {
            // in case the process found is the requested, increments
            // the number of running processes (count variable)
            if (!strcmp(exeName, processEntry.szExeFile)) { (*count)++; }

            // retrieves information about the next
            // process recorded in a system snapshot
            processRetrievalResult = Process32Next(hSnapshot, &processEntry);
        }

        // in case there was an error or there are no more
        // files to be retrieved
        if (processRetrievalResult != TRUE && GetLastError() != ERROR_NO_MORE_FILES)
            success = false;

        // closes the snapshot handle
        CloseHandle(hSnapshot);
    }
    else {
        // unsets the success flag
        success = false;
    }

    // returns
    return success;
}
