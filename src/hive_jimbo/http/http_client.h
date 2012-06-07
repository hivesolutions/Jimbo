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
        static const int default_port = 80;

        /**
         * The http socket to be used.
         */
        SOCKET http_socket;

        /**
         * The address (string) to be used, this
		 * is the tcp address or dns name for
		 * the connection.
         */
        std::string address;

        /**
         * The port to be used, this is the
		 * tcp port for the connection.
         */
        int port;

		/**
		 * The url to the content to be retrieved,
		 * this is the target url of the client.
		 */
        std::string content_url;

		/**
		 * The buffer containing the contents retrieved
		 * by the client connection.
		 */
        char *message_buffer;

		/**
		 * The size in bytes of the message retrieved
		 * from the server.
		 */
        int message_size;

		/**
		 * The size (length) in bytes of the content part
		 * of the http message.
		 */
        int content_length;

        void ParseUrl(std::string &url);
        void RetrieveData();
        void OpenConnection(std::string &address, int port);
        void CloseConnection();
    public:
        /**
         * Constructor of the class.
         */
        JB_EXPORT JBHttpClient();

        /**
         * Destructor of the class.
         */
        JB_EXPORT ~JBHttpClient();
        JB_EXPORT void GetContents(std::string &url);
        JB_EXPORT char *GetMessageBuffer();
        JB_EXPORT int GetMessageSize();
        JB_EXPORT int GetContentLength();
};
