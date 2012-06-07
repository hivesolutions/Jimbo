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

#include "http_client.h"

JBHttpClient::JBHttpClient() : JBObservable() {
    this->message_buffer = NULL;
}

JBHttpClient::~JBHttpClient() {
    // in case the message buffer is defined
    if(this->message_buffer) {
        // releases the message buffer
        free(message_buffer);
    }
}

void JBHttpClient::ParseUrl(std::string &url) {
    int index;
    int final_index;

    // allocates space for the content url
    std::string content_url;

    // finds the url part of the address
    index = url.find("http://");

    final_index = index + 7;

    if(index == std::string::npos) {
        throw "Invalid address";
    }

    std::string sub_url = url.substr(final_index);

    index = sub_url.find("/");

    std::string address = sub_url.substr(0, index);

    int port_index = address.find(":");

    if(port_index == std::string::npos) {
        this->address = address;
        this->port = JBHttpClient::default_port;
    }
    else {
        this->address = address.substr(0, port_index);
        std::string port_string = address.substr(port_index + 1);
        this->port = atoi((char *) port_string.c_str());
    }

    final_index = index + 1;

    if(index == std::string::npos) { this->content_url = "/"; }
    else { this->content_url = sub_url.substr(index); }
}

void JBHttpClient::RetrieveData() {
    int start_line_index;
    int start_header_index;
    int end_header_index;
    int start_message_index;
    int message_size;
    unsigned int total_data_length = 0;
    std::string receive_stream_string;

    // creates the socket data to be sent
    std::string socket_data = "GET " + this->content_url + " HTTP/1.1\r\n" +
        "Host: " + this->address + "\r\n" +
        "Connection: keep-alive\r\n" +
        "User-Agent: " + HTTP_AGENT_NAME + "/" + HTTP_AGENT_VERSION + "\r\n" + "\r\n";

    // sends the request and retrieves the sent length
    unsigned int data_length = send(this->http_socket, socket_data.c_str(), socket_data.length(), 0);

    // in case there was an error sending the message
    if(data_length < socket_data.length()) {
        // throws an exception
        throw "Problem sending data";
    }

    // creates the start line loaded flag
    bool start_line_loaded = false;

    // creates the header loaded flag
    bool header_loaded = false;

    // creates the message loaded
    bool message_loaded = false;

    // creates the receive stream
    std::stringstream receive_stream = std::stringstream(std::stringstream::in | std::stringstream::out | std::stringstream::binary);

    while(1) {
        // allocates memory for the receiving buffer
        char receive_buffer[HTTP_BUFFER_LENGTH + 1];

        // receives data from the host
        unsigned int data_length = recv(this->http_socket, receive_buffer, HTTP_BUFFER_LENGTH * sizeof(char), 0);

        // increments the total data length
        total_data_length += data_length;

        // writes the receive buffer to the receive stream
        receive_stream.write(receive_buffer, data_length);

        // in case the header is not loaded
        if(!header_loaded)
            receive_stream_string = receive_stream.str() + "\0";

        // in case the start line is not loaded
        if(!start_line_loaded) {
            // retrieves the start line index
            start_line_index = receive_stream_string.find("\r\n");

            if(start_line_index != std::string::npos) {
                // sets the start line loaded flag and retrieves
                // the start line header string value
                start_line_loaded = true;
                std::string start_line_header = receive_stream_string.substr(0, start_line_index);

                // allocates space for the list that will hold the various
                // tokens from the start line and then tokenizes the start
                // line header part arround the space character
                std::vector<std::string> tokens_list;
                JBTokenizer::TokenizeNoEscape(tokens_list, start_line_header, JBIsFromStringNoEscape(" "));

                // in case the header is smaller than expected, the expected
                // tokens are the protocol version, status code and status
                // message (the status message is of variable length)
                if(tokens_list.size() < 3) {
                    // throws an exception
                    throw "Invalid header in the request";
                }

                // retrieves the protocol version and the status code
                // as the first and second elements of the tokens list
                std::string protocol_version = tokens_list[0];
                std::string status_code = tokens_list[1];

                // retrieves the status message
                std::string status_message = std::string();

                // retrieves the tokens list iterator
                std::vector<std::string>::iterator iterator = tokens_list.begin();

                // increments the tokens list iterator (status code and message values)
                // they should be skipped in order to correcly join the status message
                iterator += 2;

                // sets the is first flag
                bool is_first = true;

                // iterates over all the tokens to join the complete
                // status message from the remaining tokens in the
                // tokens list
                while(iterator != tokens_list.end()) {
                    // in case it's the first iteration
                    // unsets the is first flag
                    if(is_first) { is_first = false; }
                    // otherwise it's not the first operation
                    // and adds a space to the status message
                    else { status_message += " "; }

                    // appends the current token value to the status
                    // message and increments the tokens list iterator
                    status_message += *iterator;
                    iterator++;
                }
            }
        }

        // in case the header is not loaded
        if(!header_loaded) {
            // fins the end header index
            end_header_index = receive_stream_string.find("\r\n\r\n");

            // in case the end header index is valid
            if(end_header_index != std::string::npos) {
                // sets the header loaded flag, calculates the start header
                // index and retrieves the associated header string with the
                // start header index value in mind
                header_loaded = true;
                start_header_index = start_line_index + 2;
                std::string headers = receive_stream_string.substr(start_header_index, end_header_index - start_header_index);

                // allocates space for the list that will hold the various
                // tokens from the various header lines and then tokenizes
                // the header string part arround the new line and carriage
                // return characters
                std::vector<std::string> tokens_list;
                JBTokenizer::TokenizeNoEscape(tokens_list, headers, JBIsFromStringNoEscape("\r\n"));

                // creats the map that will hold the various header values indexed
                // by their key name and associated with their string value
                std::map<std::string, std::string> headers_map = std::map<std::string, std::string>();

                // iterates over all the tokens in the tokens list to populate the
                // headers map with the resulting values from their splitting
                for(std::vector<std::string>::iterator iterator = tokens_list.begin(); iterator < tokens_list.end(); iterator++) {
                    // retrieves the current iterator position as the
                    // current token in iteration
                    std::string token = *iterator;

                    // searches for the header value, key/value separator
                    // position to use it to split the token
                    int base_index = token.find(":");

                    // retrieves the header key and values by splitting
                    // the token arround the key/value separator
                    std::string header_key = token.substr(0, base_index);
                    std::string header_value = token.substr(base_index + 1);

                    // trims the header key and values (avoids erroneous extra
                    // space characters) and sets the header in the headers map
                    JBString::Trim(header_key);
                    JBString::Trim(header_value);
                    headers_map[header_key] = header_value;
                }

                if(headers_map.find("Content-Length") != headers_map.end()) {
                    message_size = atoi(headers_map["Content-Length"].c_str());

                    // sets the content length
                    this->content_length = message_size;
                }
                else if(headers_map.find("Content-length") != headers_map.end()) {
                    message_size = atoi(headers_map["Content-length"].c_str());

                    // sets the content length
                    this->content_length = message_size;
                }

                // fires the header loaded event
                this->FireEvent(std::string("header_loaded"), &(this->content_length));
            }
        }

        // in case the message is not loaded and the header is loaded
        if(!message_loaded && header_loaded) {
            // calculates the start message index
            start_message_index = end_header_index + 4;

            // calculates the current message size
            int current_message_size = total_data_length - start_message_index;

            // fires the download changed event
            this->FireEvent(std::string("download_changed"), &current_message_size);

            // in case the current message size is the same as the defined message size
            if(current_message_size == message_size) {
                // sets the message loaded flag
                message_loaded = true;

                // calculates the message size
                int message_buffer_string_size = message_size * sizeof(char);

                // allocates space for the message buffer
                this->message_buffer = (char *) malloc(message_buffer_string_size);

                // seeks the receive stream into the message part
                receive_stream.seekg(start_message_index);

                // reads the receive stream to the message buffer
                receive_stream.read(this->message_buffer, message_buffer_string_size);

                // sets the message size
                this->message_size = message_buffer_string_size;
            }
        }

        // in case the start line is loaded the header is loaded
        // and the message is loaded, the download is complete
        if(start_line_loaded && header_loaded && message_loaded) {
            // breaks the loop
            break;
        }
    }

    // fires the download completed event
    this->FireEvent(std::string("download_completed"), NULL);
}

void JBHttpClient::OpenConnection(std::string &address, int port) {
    // the windows socket api data
    WSADATA wsa_data;

    // the socket to be used in http
    SOCKET http_socket;

    // the http socket address
    SOCKADDR_IN http_socket_address;

    // tries to startup the windows socket api
    if(WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
        // raises an exception
        throw "Socket Initialization Error";
    }

    // creates the new socket
    http_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // in case the socket created successfully
    if(http_socket == INVALID_SOCKET) {
        // raises an exception
        throw "Socket creation unsuccessfull";
    }

    // resets the http socket address
    memset(&http_socket_address, 0, sizeof(http_socket_address));

    // retrieves the host from the address
    hostent *host = gethostbyname(address.c_str());

    // retrieves the ip address of the host
    char *ip = inet_ntoa(*(struct in_addr *) *host->h_addr_list);

    // sets the http socket address
    http_socket_address.sin_family = AF_INET;
    http_socket_address.sin_port = htons(port);
    http_socket_address.sin_addr.s_addr = inet_addr(ip);

    // connects the socket
    int connection = connect(http_socket, (SOCKADDR *) &http_socket_address, sizeof(SOCKADDR_IN));

    // in case there is a problem connecting the host
    if(connection != 0) {
        // throws an exception
        throw "Problem connecting the host";
    }

    // sets the http socket in the instance
    this->http_socket = http_socket;
}

void JBHttpClient::CloseConnection() {
    // closes the socket
    closesocket(this->http_socket);
}

void JBHttpClient::GetContents(std::string &url) {
    // parses the url
    this->ParseUrl(url);

    // opens the connection in the parser address and port
    this->OpenConnection(this->address, this->port);

    try {
        // retrieves the data
        this->RetrieveData();
    }
    catch(char *) {
        // closes the connection
        this->CloseConnection();

        // rethrows exception
        throw;
    }

    // closes the connection
    this->CloseConnection();
}

char *JBHttpClient::GetMessageBuffer() {
    return this->message_buffer;
}

int JBHttpClient::GetMessageSize() {
    return this->message_size;
}

int JBHttpClient::GetContentLength() {
    return this->content_length;
}
