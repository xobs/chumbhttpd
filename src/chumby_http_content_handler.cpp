/*
 * chumby::HTTPContentHandler
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

#include <chumby_httpd/chumby_http_content_handler.h>
#include <iostream>
#include <fstream>

chumby::HTTPResponse * chumby::HTTPEchoContentHandler::handleRequest(const chumby::HTTPRequest & request)
{
	chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);
	std::string mimeType = "text/plain";
	std::string uri = request.getRequestURI();
	response->setMimeType(mimeType);
	response->addContent(uri);
	return response;
}

chumby::HTTPDefaultContentHandler::HTTPDefaultContentHandler(std::string &resp,std::string &mimeType)
{
	_response = resp;
	_mimeType = mimeType;
}

chumby::HTTPDefaultContentHandler::HTTPDefaultContentHandler(const char *resp,const char *mimeType)
{
	_response = resp;
	_mimeType = mimeType;
}

chumby::HTTPResponse * chumby::HTTPDefaultContentHandler::handleRequest(const chumby::HTTPRequest & request)
{
	chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);
	response->setMimeType(_mimeType);
	response->addContent(_response);
	return response;
}

chumby::HTTPSimpleContentHandler::HTTPSimpleContentHandler(std::string &path,std::string &resp,std::string &mimeType)
	: HTTPDefaultContentHandler(resp,mimeType)
{
	_path = path;
}

chumby::HTTPSimpleContentHandler::HTTPSimpleContentHandler(const char *path,const char *resp,const char *mimeType)
	: HTTPDefaultContentHandler(resp,mimeType)
{
	_path = path;
}

chumby::HTTPResponse * chumby::HTTPSimpleContentHandler::handleRequest(const chumby::HTTPRequest & request)
{
	if (request.getRequestURI()==_path) {
		chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);
		response->setMimeType(_mimeType);
		response->addContent(_response);
		return response;
	}
	return NULL;
}

chumby::HTTPSimpleFileContentHandler::HTTPSimpleFileContentHandler(std::string &path,std::string &filePath,std::string &mimeType)
{
	_path = path;
	_filePath = filePath;
	_mimeType = mimeType;
}

chumby::HTTPSimpleFileContentHandler::HTTPSimpleFileContentHandler(const char *path,const char *filePath,const char *mimeType)
{
	_path = path;
	_filePath = filePath;
	_mimeType = mimeType;
}

chumby::HTTPResponse * chumby::HTTPSimpleFileContentHandler::handleRequest(const chumby::HTTPRequest & request)
{
	if (request.getRequestURI()==_path) {
		chumby::HTTPResponse *response = new chumby::HTTPResponse(chumby::HTTP_RESPONSE_CODE_OKAY);
		response->setMimeType(_mimeType);
		std::ifstream filestr;
		filestr.open(_filePath.c_str());
		std::filebuf * pbuf = filestr.rdbuf();
		long size = pbuf->pubseekoff (0, std::ios::end, std::ios::in | std::ios::binary);
		char * buffer = new char[size];
		pbuf->pubseekpos (0, std::ios::in | std::ios::binary);
		pbuf->sgetn (buffer, size);
		filestr.close();
		response->addContent(buffer, size);
		delete [] buffer;
		return response;
	}
	return NULL;
}
