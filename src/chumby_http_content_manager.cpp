/*
 * chumby::HTTPContentManager
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

#include <chumby_httpd/chumby_http_content_manager.h>

#include <iostream>

chumby::HTTPContentManager::HTTPContentManager()
{
  _contentHandlers = new std::list<chumby::HTTPContentHandler *>();
}

chumby::HTTPContentManager::~HTTPContentManager()
{
	if (_contentHandlers) {
		delete _contentHandlers;
	}
}

void chumby::HTTPContentManager::addContentHandler(chumby::HTTPContentHandler * contentHandler)
{
	_contentHandlers->push_back(contentHandler);
}

chumby::HTTPResponse *chumby::HTTPContentManager::handleRequest(const chumby::HTTPRequest & request)
{
	std::string requestURI = request.getRequestURI();
	chumby::HTTPResponse * response = NULL;

	//std::cout << "HTTPContentManager: processing '" << requestURI << "'" << std::endl;

	std::list<chumby::HTTPContentHandler *>::const_iterator it;
	for (it=_contentHandlers->begin(); it!=_contentHandlers->end();++it) {
		response = (*it)->handleRequest(request);
		if (response) {
			break;
		}
	}
	if (!response) {
		response = new chumby::HTTPResponse(HTTP_RESPONSE_CODE_NOT_FOUND);
		std::string notfound = "<html>\n<head><title>Not Found</title></head><body><h1>Not Found</h1></body></html>";
		response->addContent(notfound);
		std::string mimeType = "text/html";
		response->setMimeType(mimeType);
	}
	return response;
}
