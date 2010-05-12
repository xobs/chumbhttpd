/*
 * chumby::HTTPConnection
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

#include <chumby_httpd/chumby_http_connection.h>

#include <stdexcept>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFFER_SIZE 10000

chumby::HTTPConnection::HTTPConnection(int fd, chumby::HTTPContentManager * contentManager)
	: chumby::Thread(), _socketfd(fd), _contentManager(contentManager)
{
	start();
}

chumby::HTTPConnection::~HTTPConnection()
{
	close(_socketfd);
}

void * chumby::HTTPConnection::run()
{
	chumby::HTTPRequest * request = receiveRequest();
	chumby::HTTPResponse * response = _contentManager->handleRequest(*request);
	char * response_str = response->getResponseString();
	size_t response_len = response->getResponseLength();

	if (send(_socketfd, response_str, response_len, 0) == -1) {
		std::cout << "Send error." << std::endl;
	}
	delete request;
	delete response;
	delete [] response_str;
	delete this;
	return 0;
}

chumby::HTTPRequest *chumby::HTTPConnection::receiveRequest()
{
	std::string result;

	char buf[BUFFER_SIZE];
	int numbytes = 0;
	bool again = true;

	while (again) {
		if ((numbytes = recv(_socketfd, buf, BUFFER_SIZE - 1, 0)) == -1) {
			throw std::runtime_error("Error while receiving.");
		}
		buf[numbytes] = '\0';
		result += buf;
		if (numbytes != BUFFER_SIZE - 1) {
			again = false;
		}
	}
	return new chumby::HTTPRequest(result);
}
