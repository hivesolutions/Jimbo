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

#include "http_client.h"

JBHttpClient::JBHttpClient() : JBObservable() {
    this->messageBuffer = NULL;
}

JBHttpClient::~JBHttpClient() {
    // in case the message buffer is defined
    if(this->messageBuffer) {
        // releases the message buffer
        free(messageBuffer);
    }
}

void JBHttpClient::parseUrl(std::string &url) {
    int index;
    int finalIndex;

    // allocates space for the content url
    std::string contentUrl;

    // finds the url part of the address
    index = url.find("http://");

    finalIndex = index + 7;

    if(index == std::string::npos) {
        throw "Invalid address";
    }

    std::string subUrl = url.substr(finalIndex);

    index = subUrl.find("/");

    std::string address = subUrl.substr(0, index);

    int portIndex = address.find(":");

    if(portIndex == std::string::npos) {
        this->address = address;
        this->port = JBHttpClient::defaultPort;
    }
    else {
        this->address = address.substr(0, portIndex);
        std::string portString = address.substr(portIndex + 1);
        this->port = atoi((char *) portString.c_str());
    }

    finalIndex = index + 1;

    if(index == std::string::npos) { this->contentUrl = "/"; }
    else { this->contentUrl = subUrl.substr(index); }
}

void JBHttpClient::retrieveData() {
    int startLineIndex;
    int startHeaderIndex;
    int endHeaderIndex;
    int startMessageIndex;
    int messageSize;
    unsigned int totalDataLength = 0;
    std::string receiveStreamString;

    // creates the socket data to be sent
    std::string socketData = "GET " + this->contentUrl + " HTTP/1.1\r\n" +
                             "Host: " + this->address + "\r\n" +
                             "Connection: keep-alive\r\n" +
                             "User-Agent: " + HTTP_AGENT_NAME + "/" + HTTP_AGENT_VERSION + "\r\n" + "\r\n";

    // sends the request and retrieves the sent length
    unsigned int dataLength = send(this->httpSocket, socketData.c_str(), socketData.length(), 0);

    // in case there was an error sending the message
    if(dataLength < socketData.length()) {
        // throws an exception
        throw "Problem sending data";
    }

    // creates the start line loaded flag
    bool startLineLoaded = false;

    // creates the header loaded flag
    bool headerLoaded = false;

    // creates the message loaded
    bool messageLoaded = false;

    // creates the receive stream
    std::stringstream receiveStream = std::stringstream(std::stringstream::in | std::stringstream::out | std::stringstream::binary);

    while(1) {
        // allocates memory for the receiving buffer
        char receiveBuffer[HTTP_BUFFER_LENGTH + 1];

        // receives data from the host
        unsigned int dataLength = recv(this->httpSocket, receiveBuffer, HTTP_BUFFER_LENGTH * sizeof(char), 0);

        // increments the total data length
        totalDataLength += dataLength;

        // writes the receive buffer to the receive stream
        receiveStream.write(receiveBuffer, dataLength);

        // in case the header is not loaded
        if(!headerLoaded)
            receiveStreamString = receiveStream.str() + "\0";

        // in case the start line is not loaded
        if(!startLineLoaded) {
            // retrieves the start line index
            startLineIndex = receiveStreamString.find("\r\n");

            if(startLineIndex != std::string::npos) {
                // sets the start line loaded flag and retrieves
                // the start line header string value
                startLineLoaded = true;
                std::string startLineHeader = receiveStreamString.substr(0, startLineIndex);

                // allocates space for the list that will hold the various
                // tokens from the start line and then tokenizes the start
                // line header part arround the space character
                std::vector<std::string> tokensList;
                JBTokenizer::TokenizeNoEscape(tokensList, startLineHeader, JBIsFromStringNoEscape(" "));

                // in case the header is smaller than expected, the expected
                // tokens are the protocol version, status code and status
                // message (the status message is of variable length)
                if(tokensList.size() < 3) {
                    // throws an exception
                    throw "Invalid header in the request";
                }

                // retrieves the protocol version and the status code
                // as the first and second elements of the tokens list
                std::string protocolVersion = tokensList[0];
                std::string statusCode = tokensList[1];

                // retrieves the status message
                std::string statusMessage = std::string();

                // retrieves the tokens list iterator
                std::vector<std::string>::iterator iterator = tokensList.begin();

                // increments the tokens list iterator (status code and message values)
                // they should be skipped in order to correcly join the status message
                iterator += 2;

                // sets the is first flag
                bool isFirst = true;

                // iterates over all the tokens to join the complete
                // status message from the remaining tokens in the
                // tokens list
                while(iterator != tokensList.end()) {
                    // in case it's the first iteration
                    // unsets the is first flag
                    if(isFirst) { isFirst = false; }
                    // otherwise it's not the first operation
                    // and adds a space to the status message
                    else { statusMessage += " "; }

                    // appends the current token value to the status
                    // message and increments the tokens list iterator
                    statusMessage += *iterator;
                    iterator++;
                }
            }
        }

        // in case the header is not loaded
        if(!headerLoaded) {
            // fins the end header index
            endHeaderIndex = receiveStreamString.find("\r\n\r\n");

            // in case the end header index is valid
            if(endHeaderIndex != std::string::npos) {
                // sets the header loaded flag, calculates the start header
                // index and retrieves the associated header string with the
                // start header index value in mind
                headerLoaded = true;
                startHeaderIndex = startLineIndex + 2;
                std::string headers = receiveStreamString.substr(startHeaderIndex, endHeaderIndex - startHeaderIndex);

                // allocates space for the list that will hold the various
                // tokens from the various header lines and then tokenizes
                // the header string part arround the new line and carriage
                // return characters
                std::vector<std::string> tokensList;
                JBTokenizer::TokenizeNoEscape(tokensList, headers, JBIsFromStringNoEscape("\r\n"));

                // creats the map that will hold the various header values indexed
                // by their key name and associated with their string value
                std::map<std::string, std::string> headersMap = std::map<std::string, std::string>();

                // iterates over all the tokens in the tokens list to populate the
                // headers map with the resulting values from their splitting
                for(std::vector<std::string>::iterator iterator = tokensList.begin(); iterator < tokensList.end(); iterator++) {
                    // retrieves the current iterator position as the
                    // current token in iteration
                    std::string token = *iterator;

                    // searches for the header value, key/value separator
                    // position to use it to split the token
                    int baseIndex = token.find(":");

                    // retrieves the header key and values by splitting
                    // the token arround the key/value separator
                    std::string headerKey = token.substr(0, baseIndex);
                    std::string headerValue = token.substr(baseIndex + 1);

                    // trims the header key and values (avoids erroneous extra
                    // space characters) and sets the header in the headers map
                    JBString::trim(headerKey);
                    JBString::trim(headerValue);
                    headersMap[headerKey] = headerValue;
                }

                if(headersMap.find("Content-Length") != headersMap.end()) {
                    messageSize = atoi(headersMap["Content-Length"].c_str());

                    // sets the content length
                    this->contentLength = messageSize;
                }
                else if(headersMap.find("Content-length") != headersMap.end()) {
                    messageSize = atoi(headersMap["Content-length"].c_str());

                    // sets the content length
                    this->contentLength = messageSize;
                }

                // fires the header loaded event
                this->fireEvent(std::string("header_loaded"), &(this->contentLength));
            }
        }

        // in case the message is not loaded and the header is loaded
        if(!messageLoaded && headerLoaded) {
            // calculates the start message index
            startMessageIndex = endHeaderIndex + 4;

            // calculates the current message size
            int currentMessageSize = totalDataLength - startMessageIndex;

            // fires the download changed event
            this->fireEvent(std::string("download_changed"), &currentMessageSize);

            // in case the current message size is the same as the defined message size
            if(currentMessageSize == messageSize) {
                // sets the message loaded flag
                messageLoaded = true;

                // calculates the message size
                int messageBufferStringSize = messageSize * sizeof(char);

                // allocates space for the message buffer
                this->messageBuffer = (char *) malloc(messageBufferStringSize);

                // seeks the receive stream into the message part
                receiveStream.seekg(startMessageIndex);

                // reads the receive stream to the message buffer
                receiveStream.read(this->messageBuffer, messageBufferStringSize);

                // sets the message size
                this->messageSize = messageBufferStringSize;
            }
        }

        // in case the start line is loaded the header is loaded
        // and the message is loaded, the download is complete
        if(startLineLoaded && headerLoaded && messageLoaded) {
            // breaks the loop
            break;
        }
    }

    // fires the download completed event
    this->fireEvent(std::string("download_completed"), NULL);
}

void JBHttpClient::openConnection(std::string &address, int port) {
    // the windows socket api data
    WSADATA wsaData;

    // the socket to be used in http
    SOCKET httpSocket;

    // the http socket address
    SOCKADDR_IN httpSocketAddress;

    // tries to startup the windows socket api
    if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        // raises an exception
        throw "Socket Initialization Error";
    }

    // creates the new socket
    httpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // in case the socket created successfully
    if(httpSocket == INVALID_SOCKET) {
        // raises an exception
        throw "Socket creation unsuccessfull";
    }

    // resets the http socket address
    memset(&httpSocketAddress, 0, sizeof(httpSocketAddress));

    // retrieves the host from the address
    hostent *host = gethostbyname(address.c_str());

    // retrieves the ip address of the host
    char *ip = inet_ntoa(*(struct in_addr *) *host->h_addr_list);

    // sets the http socket address
    httpSocketAddress.sin_family = AF_INET;
    httpSocketAddress.sin_port = htons(port);
    httpSocketAddress.sin_addr.s_addr = inet_addr(ip);

    // connects the socket
    int connection = connect(httpSocket, (SOCKADDR *) &httpSocketAddress, sizeof(SOCKADDR_IN));

    // in case there is a problem connecting the host
    if(connection != 0) {
        // throws an exception
        throw "Problem connecting the host";
    }

    // sets the http socket in the instance
    this->httpSocket = httpSocket;
}

void JBHttpClient::closeConnection() {
    // closes the socket
    closesocket(this->httpSocket);
}

void JBHttpClient::getContents(std::string &url) {
    // parses the url
    this->parseUrl(url);

    // opens the connection in the parser address and port
    this->openConnection(this->address, this->port);

    try {
        // retrieves the data
        this->retrieveData();
    }
    catch(char *) {
        // closes the connection
        this->closeConnection();

        // rethrows exception
        throw;
    }

    // closes the connection
    this->closeConnection();
}

char *JBHttpClient::getMessageBuffer() {
    return this->messageBuffer;
}

int JBHttpClient::getMessageSize() {
    return this->messageSize;
}

int JBHttpClient::getContentLength() {
    return this->contentLength;
}
