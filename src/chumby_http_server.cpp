/*
 * chumby::HTTPServer
 * Copyright (C) Chumby Industries, 2007
 *
 * based on Artemis Copyright (C) 2007 Andreas Buechele
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <chumby_httpd/chumby_http_server.h>
#include <chumby_httpd/chumby_http_connection.h>

#include <stdexcept>
#include <iostream>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

chumby::HTTPServer::HTTPServer(int port, int public_server)
{
	_contentManager = new chumby::HTTPContentManager();
	_yes = 1; // ????
	int _backlog = 10; // ????
	if ((_socketfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
		throw std::runtime_error("Error while socket call.");
	}

	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR, &_yes, sizeof(int)) == -1) {
		throw std::runtime_error("Error while setsockopt call.");
	}

	_local_in_addr.sin_family = AF_INET; // host byte order
	_local_in_addr.sin_port = htons(port); // short, network byte order
	if(public_server)
		// automatically fill with my IP
		_local_in_addr.sin_addr.s_addr = INADDR_ANY;
	else
		// localhost only
		_local_in_addr.sin_addr.s_addr = htonl(0x7f000001);
	memset(&(_local_in_addr.sin_zero), '\0', 8); // zero the rest of the struct  

	if (bind(_socketfd, (struct sockaddr *)&_local_in_addr, sizeof(struct sockaddr)) == -1) {
		throw std::runtime_error("Error while bind call.");
	}

	if (listen(_socketfd, _backlog) == -1) {
		throw std::runtime_error("Error while listen call.");
	}
}

chumby::HTTPServer::~HTTPServer()
{
	if (_contentManager) {
		delete _contentManager;
	}
}

void chumby::HTTPServer::start()
{
	socklen_t sin_size;
	struct sockaddr_in client_addr; // connector's address information
	int new_fd;

	chumby::HTTPConnection * connection;

	//std::cout << "HTTPServer::start()" << std::endl;
	while(true) {
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(_socketfd, (struct sockaddr *)&client_addr, &sin_size)) == -1) {
			std::cout << "Error while accepting." << std::endl;
			continue;
		}
		connection = new chumby::HTTPConnection(new_fd, _contentManager);
	}
}

void chumby::HTTPServer::addContentHandler(chumby::HTTPContentHandler * contentHandler)
{
	_contentManager->addContentHandler(contentHandler);
}

