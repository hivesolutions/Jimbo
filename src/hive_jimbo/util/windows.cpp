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

char **JBWindows::CommandLineToArgv(char *command_line, int *argc) {
    int retval;
    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, command_line, -1, NULL, 0);
    if(!SUCCEEDED(retval)) {
        return NULL;
    }

    LPWSTR lpWideCharStr = (LPWSTR) malloc(retval * sizeof(WCHAR));
    if(lpWideCharStr == NULL) {
        return NULL;
    }

    retval = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, command_line, -1, lpWideCharStr, retval);
    if(!SUCCEEDED(retval)) {
        free(lpWideCharStr);
        return NULL;
    }

    int num_args;
    LPWSTR *args;
    args = CommandLineToArgvW(lpWideCharStr, &num_args);
    free(lpWideCharStr);
    if (args == NULL)
        return NULL;

    int storage = num_args * sizeof(LPSTR);

    for(int index = 0; index < num_args; ++index) {
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

    int buf_len = storage - num_args * sizeof(LPSTR);
    LPSTR buffer = ((LPSTR)result) + num_args * sizeof(LPSTR);
    for(int index = 0; index < num_args; ++index) {
        BOOL lpUsedDefaultChar = FALSE;
        retval = WideCharToMultiByte(CP_ACP, 0, args[index], -1, buffer, buf_len, NULL, &lpUsedDefaultChar);
        if(!SUCCEEDED(retval)) {
            LocalFree(result);
            LocalFree(args);
            return NULL;
        }

        result[index] = buffer;
        buffer += retval;
        buf_len -= retval;
    }

    LocalFree(args);

    *argc = num_args;
    return result;
}

int JBWindows::CopyRecursiveShell(const std::string &target_directory, const std::string &source_directory) {
    // retrieves the length of both the target and source
    // directory length do that it's possible to allocate
    // the structure native string structures
    size_t target_directory_length = target_directory.length();
    size_t source_directory_length = source_directory.length();

    // creates the target directory native string from the
    // target directory string and puts the double string
    // termination characters
    char *target_directory_string = (char *) malloc(target_directory.length() + 2);
    memcpy(target_directory_string, target_directory.c_str(), target_directory_length);
    target_directory_string[target_directory_length] = '\0';
    target_directory_string[target_directory_length + 1] = '\0';

    // creates the source directory native string from the
    // source directory string and puts the double string
    // termination characters, note that the wildcard character
    // is put in the string so that all files are copy
    char *source_directory_string = (char *) malloc(source_directory.length() + 4);
    memcpy(source_directory_string, source_directory.c_str(), source_directory_length);
    source_directory_string[source_directory_length] = '\\';
    source_directory_string[source_directory_length + 1] = '*';
    source_directory_string[source_directory_length + 2] = '\0';
    source_directory_string[source_directory_length + 3] = '\0';

    // creates the shell operation structure for the recursive
    // copy of the source to target directories
    SHFILEOPSTRUCT operation = { 0 };
    operation.hwnd = NULL;
    operation.wFunc = FO_COPY;
    operation.pTo = target_directory_string;
    operation.pFrom = source_directory_string;
    operation.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;
    SHFileOperation(&operation);

    // releases the space allocated for the target
    // and source directory strings
    free(target_directory_string);
    free(source_directory_string);

    return 0;
}

int JBWindows::DeleteRecursive(const std::string &target_directory, bool delete_subdirectories) {
    bool subdirectory = false;
    HANDLE handler_file;
    std::string file_path;
    std::string pattern;
    WIN32_FIND_DATA file_information;

    pattern = target_directory + "\\*.*";
    handler_file = FindFirstFile(pattern.c_str(), &file_information);

    // in case the file handler is not invalid
    if(handler_file != INVALID_HANDLE_VALUE) {
        do {
            if(file_information.cFileName[0] != '.') {
                file_path.erase();
                file_path = target_directory + "\\" + file_information.cFileName;

                if(file_information.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if(delete_subdirectories) {
                        // deletes the subdirectory
                        int return_value = JBWindows::DeleteRecursive(file_path, delete_subdirectories);

                        // in case the return value is invalid
                        if(return_value)
                            // returns the return value
                            return return_value;
                    }
                    else { subdirectory = true; }
                }
                else {
                    // sets the file attributes
                    if(SetFileAttributes(file_path.c_str(), FILE_ATTRIBUTE_NORMAL) == false)
                        // returns the last error
                        return GetLastError();

                    // deletes the file
                    if(DeleteFile(file_path.c_str()) == false)
                        // returns the last error
                        return GetLastError();
                }
            }
        } while(FindNextFile(handler_file, &file_information) == TRUE);

        // closes the file Handle
        FindClose(handler_file);

        DWORD error = GetLastError();
        if(error != ERROR_NO_MORE_FILES) { return error; }
        else {
            if(!subdirectory) {
                // sets the directory attributes, in case of failuree
                // returns immediately in error
                if(SetFileAttributes(target_directory.c_str(), FILE_ATTRIBUTE_NORMAL) == false) { return GetLastError(); }

                // deletes the directory, in case of failuree
                // returns immediately in error
                if(RemoveDirectory(target_directory.c_str()) == false) { return GetLastError(); }
            }
        }
    }

    return 0;
}

int JBWindows::DeleteRecursiveShell(const char *target_directory, bool recycle_bin) {
    // retrieves the target directory length
    int target_directory_length = strlen(target_directory);

    // allocates space
    TCHAR *target_directory_aux = new TCHAR[target_directory_length + 2];

    strcpy_s(target_directory_aux, target_directory_length + 1, target_directory);

    target_directory_aux[target_directory_length] = 0;
    target_directory_aux[target_directory_length + 1] = 0;

    SHFILEOPSTRUCT operation;
    operation.hwnd = NULL;

    // sets the operation as delete
    operation.wFunc = FO_DELETE;

    // sets the source file name as double null terminated string
    operation.pFrom = target_directory_aux;

    // no destination is required
    operation.pTo = NULL;

    // do not prompt the user
    operation.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

    if(recycle_bin) { operation.fFlags |= FOF_ALLOWUNDO; }

    operation.fAnyOperationsAborted = FALSE;
    operation.lpszProgressTitle = NULL;
    operation.hNameMappings = NULL;

    int return_value = SHFileOperation(&operation);

    delete [] target_directory_aux;

    return return_value;
}

int JBWindows::GetVersion() {
    // allocates space for the version variable
    unsigned long version;

    // retrievesthe current windows version
    version = GetVersion();

    // returns the major version
    return LOBYTE(LOWORD(version));
}
