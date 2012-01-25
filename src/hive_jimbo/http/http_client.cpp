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

CHttpClient::CHttpClient() : CObservable() {
    this->messageBuffer = NULL;
}

CHttpClient::~CHttpClient() {
    // in case the message buffer is defined
    if(this->messageBuffer) {
        // releases the message buffer
        free(messageBuffer);
    }
}

void CHttpClient::parseUrl(std::string &url) {
    int index;
    int finalIndex;

    // allocates space for the content url
    std::string contentUrl;

    // finds the url part of the address
    index = url.find("http://");

    finalIndex = index + 7;

    if(index == std::string::npos)
        throw "Invalid address";

    std::string subUrl = url.substr(finalIndex);

    index = subUrl.find("/");

    std::string address = subUrl.substr(0, index);

    int portIndex = address.find(":");

    if(portIndex == std::string::npos) {
        this->address = address;
        this->port = CHttpClient::defaultPort;
    }
    else {
        this->address = address.substr(0, portIndex);
        std::string portString = address.substr(portIndex + 1);
        this->port = atoi((char *) portString.c_str());
    }

    finalIndex = index + 1;

    if(index == std::string::npos)
        this->contentUrl = "/";
    else
        this->contentUrl = subUrl.substr(index);
}

void CHttpClient::retrieveData() {
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
                // sets the start line loaded flag
                startLineLoaded = true;

                // retrieves the start line header
                std::string startLineHeader = receiveStreamString.substr(0, startLineIndex);




                std::vector<std::string> tokensList;

                // tokenizes the buffer string and puts the tokens in the tokens vector
                CTokenizer::TokenizeNoEscape(tokensList, startLineHeader, CIsFromStringNoEscape(" "));





                // in case the header is smaller than expected
                if(tokensList.size() < 3) {
                    // throws an exception
                    throw "Invalid header in the request";
                }

                // retrieves the protocol version
                std::string protocolVersion = tokensList[0];

                // retrieves the status code
                std::string statusCode = tokensList[1];

                // retrieves the status message
                std::string statusMessage = std::string();

                // retrieves the tokens list iterator
                std::vector<std::string>::iterator tokensListIterator = tokensList.begin();

                // increments the tokens list iterator (status code and message values)
                tokensListIterator += 2;

                // sets the is first flag
                bool isFirst = true;

                // iterates over all the tokens
                while(tokensListIterator != tokensList.end()) {
                    // in case it's the first iteration
                    if(isFirst) {
                        // unsets the is first flag
                        isFirst = false;
                    }
                    // otherwise it's not the first
                    else {
                        // adds a space to the status message
                        statusMessage += " ";
                    }

                    statusMessage += *tokensListIterator;
                    tokensListIterator++;
                }
            }
        }

        // in case the header is not loaded
        if(!headerLoaded) {
            // fins the end header index
            endHeaderIndex = receiveStreamString.find("\r\n\r\n");

            // in case the end header index is valid
            if(endHeaderIndex != std::string::npos) {
                // sets the header loaded flag
                headerLoaded = true;

                // calculates the start header index
                startHeaderIndex = startLineIndex + 2;

                // retrieves the headers string
                std::string headers = receiveStreamString.substr(startHeaderIndex, endHeaderIndex - startHeaderIndex);




                std::vector<std::string> tokensList;

                // tokenizes the buffer string and puts the tokens in the tokens vector
                CTokenizer::TokenizeNoEscape(tokensList, headers, CIsFromStringNoEscape("\r\n"));

                std::map<std::string, std::string> headersMap = std::map<std::string, std::string>();

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
                    headerKey = CString::trim(headerKey);
                    headerValue = CString::trim(headerValue);
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

void CHttpClient::openConnection(std::string &address, int port) {
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

void CHttpClient::closeConnection() {
    // closes the socket
    closesocket(this->httpSocket);
}

void CHttpClient::getContents(std::string &url) {
    // parses the url
    this->parseUrl(url);

    // opens the connection in the parser address and port
    this->openConnection(this->address, this->port);

    try {
        // retrieves the data
        this->retrieveData();
    }
    catch(char *exception) {
        // closes the connection
        this->closeConnection();

        // rethrows exception
        throw exception;
    }

    // closes the connection
    this->closeConnection();
}

char *CHttpClient::getMessageBuffer() {
    return this->messageBuffer;
}

int CHttpClient::getMessageSize() {
    return this->messageSize;
}

int CHttpClient::getContentLength() {
    return this->contentLength;
}
