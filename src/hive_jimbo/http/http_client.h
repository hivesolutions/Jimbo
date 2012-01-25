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

#pragma once

#include "../util/util.h"
#include "../structures/structures.h"

/**
 * The length of the buffer to be used in http.
 */
#define HTTP_BUFFER_LENGTH 4096

/**
 * The name of the agent to be used.
 */
#define HTTP_AGENT_NAME "Hive-Colony-Http-Client"

/**
 * The version fo the agent to be used.
 */
#define HTTP_AGENT_VERSION "1.0.0"

/**
 * Provides the support for http parsing and and retrieving.
 * This class represents the entry point for the http client.
 */
class JBHttpClient : public JBObservable {
    private:
        /**
         * The default port to be used.
         */
        static const int defaultPort = 80;

        /**
         * The http socket to be used.
         */
        SOCKET httpSocket;

        /**
         * The address (string) to be used.
         */
        std::string address;

        /**
         * The port to be used.
         */
        int port;
        std::string contentUrl;
        char *messageBuffer;
        int messageSize;
        int contentLength;

        void parseUrl(std::string &url);
        void retrieveData();
        void openConnection(std::string &address, int port);
        void closeConnection();
    public:
        /**
         * Constructor of the class.
         */
        JIMBO_EXPORT_PREFIX JBHttpClient();

        /**
         * Destructor of the class.
         */
        JIMBO_EXPORT_PREFIX ~JBHttpClient();
        JIMBO_EXPORT_PREFIX void getContents(std::string &url);
        JIMBO_EXPORT_PREFIX char *getMessageBuffer();
        JIMBO_EXPORT_PREFIX int getMessageSize();
        JIMBO_EXPORT_PREFIX int getContentLength();
};
