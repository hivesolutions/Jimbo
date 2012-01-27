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

#include "windows.h"

char **JBWindows::commandLineToArgv(char *commandLine, int *argc) {
    int retval;
    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, commandLine, -1, NULL, 0);
    if(!SUCCEEDED(retval)) {
        return NULL;
    }

    LPWSTR lpWideCharStr = (LPWSTR) malloc(retval * sizeof(WCHAR));
    if(lpWideCharStr == NULL) {
        return NULL;
    }

    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, commandLine, -1, lpWideCharStr, retval);
    if(!SUCCEEDED(retval)) {
        free(lpWideCharStr);
        return NULL;
    }

    int numArgs;
    LPWSTR *args;
    args = CommandLineToArgvW(lpWideCharStr, &numArgs);
    free(lpWideCharStr);
    if (args == NULL)
        return NULL;

    int storage = numArgs * sizeof(LPSTR);

    for(int index = 0; index < numArgs; ++index) {
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[index], -1, NULL, 0, NULL, &lpUsedDefaultChar);
        if(!SUCCEEDED(retval)) {
            LocalFree(args);
            return NULL;
        }

        storage += retval;
    }

    LPSTR *result = (LPSTR *)LocalAlloc(LMEM_FIXED, storage);
    if(result == NULL) {
        LocalFree(args);
        return NULL;
    }

    int bufLen = storage - numArgs * sizeof(LPSTR);
    LPSTR buffer = ((LPSTR)result) + numArgs * sizeof(LPSTR);
    for(int index = 0; index < numArgs; ++index) {
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[index], -1, buffer, bufLen, NULL, &lpUsedDefaultChar);
        if(!SUCCEEDED(retval)) {
            LocalFree(result);
            LocalFree(args);
            return NULL;
        }

        result[index] = buffer;
        buffer += retval;
        bufLen -= retval;
    }

    LocalFree(args);

    *argc = numArgs;
    return result;
}

int JBWindows::deleteRecursive(const std::string &targetDirectory, bool deleteSubdirectories) {
    bool subdirectory = false;
    HANDLE handlerFile;
    std::string filePath;
    std::string pattern;
    WIN32_FIND_DATA fileInformation;

    pattern = targetDirectory + "\\*.*";
    handlerFile = FindFirstFile(pattern.c_str(), &fileInformation);

    // in case the file handler is not invalid
    if(handlerFile != INVALID_HANDLE_VALUE) {
        do {
            if(fileInformation.cFileName[0] != '.') {
                filePath.erase();
                filePath = targetDirectory + "\\" + fileInformation.cFileName;

                if(fileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if(deleteSubdirectories) {
                        // deletes the subdirectory
                        int returnValue = JBWindows::deleteRecursive(filePath, deleteSubdirectories);

                        // in case the return value is invalid
                        if(returnValue)
                            // returns the return value
                            return returnValue;
                    }
                    else { subdirectory = true; }
                }
                else {
                    // sets the file attributes
                    if(SetFileAttributes(filePath.c_str(), FILE_ATTRIBUTE_NORMAL) == false)
                        // returns the last error
                        return GetLastError();

                    // deletes the file
                    if(DeleteFile(filePath.c_str()) == false)
                        // returns the last error
                        return GetLastError();
                }
            }
        } while(FindNextFile(handlerFile, &fileInformation) == TRUE);

        // closes the file handle
        FindClose(handlerFile);

        DWORD error = GetLastError();
        if(error != ERROR_NO_MORE_FILES) { return error; }
        else {
            if(!subdirectory) {
                // sets the directory attributes, in case of failuree
                // returns immediately in error
                if(SetFileAttributes(targetDirectory.c_str(), FILE_ATTRIBUTE_NORMAL) == false) { return GetLastError(); }

                // deletes the directory, in case of failuree
                // returns immediately in error
                if(RemoveDirectory(targetDirectory.c_str()) == false) { return GetLastError(); }
            }
        }
    }

    return 0;
}

int JBWindows::deleteRecursiveShell(const char *targetDirectory, bool recycleBin) {
    // retrieves the target directory length
    int targetDirectoryLength = strlen(targetDirectory);

    // allocates space
    TCHAR *targetDirectoryAux = new TCHAR[targetDirectoryLength + 2];

    strcpy_s(targetDirectoryAux, targetDirectoryLength + 1, targetDirectory);

    targetDirectoryAux[targetDirectoryLength] = 0;
    targetDirectoryAux[targetDirectoryLength + 1] = 0;

    SHFILEOPSTRUCT fileOperation;
    fileOperation.hwnd = NULL;

    // sets the operation as delete
    fileOperation.wFunc = FO_DELETE;

    // sets the source file name as double null terminated string
    fileOperation.pFrom = targetDirectoryAux;

    // no destination is required
    fileOperation.pTo = NULL;

    // do not prompt the user
    fileOperation.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

    if(!recycleBin) { fileOperation.fFlags |= FOF_ALLOWUNDO; }

    fileOperation.fAnyOperationsAborted = FALSE;
    fileOperation.lpszProgressTitle = NULL;
    fileOperation.hNameMappings = NULL;

    int returnValue = SHFileOperation(&fileOperation);

    delete [] targetDirectoryAux;

    return returnValue;
}

int JBWindows::getVersion() {
    // allocates space for the version variable
    unsigned long version;

    // retrievesthe current windows version
    version = GetVersion();

    // returns the major version
    return LOBYTE(LOWORD(version));
}
