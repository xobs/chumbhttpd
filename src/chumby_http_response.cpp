/*
 * chumby::HTTPResponse
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

#include <string.h>
#include <chumby_httpd/chumby_http_response.h>

#include <iostream>
#include <sstream>

chumby::HTTPResponse::HTTPResponse(HTTPResponseCode httpResponseCode)
  : _httpResponseCode(httpResponseCode),
    _httpResponseContent(0),
    _httpResponseContentLength(0),
    _httpResponseLength(0)
{
  _responseHeaders = new std::map<std::string, std::string>();

}

chumby::HTTPResponse::~HTTPResponse()
{
	if (_httpResponseContent) {
		delete [] _httpResponseContent;
	}
	if (_responseHeaders) {
		delete _responseHeaders;
	}
}

void chumby::HTTPResponse::addHeader(const char *name, const char *value)
{
    std::string str_name = name;
    std::string str_value = value;

    addHeader(str_name, str_value);
}

void chumby::HTTPResponse::addHeader(std::string &name, std::string &value)
{
	(*_responseHeaders)[name] = value;
}

void chumby::HTTPResponse::setMimeType(std::string &mimeType)
{
	std::string name = "Content-type";
	addHeader(name,mimeType);
}

void chumby::HTTPResponse::setMimeType(const char *mimeType)
{
	std::string name = "Content-type";
	std::string _mimeType = mimeType;
	addHeader(name,_mimeType);
}


void chumby::HTTPResponse::addContent(const char * content, long length)
{
	_httpResponseContentLength = length;
	_httpResponseContent = new char[_httpResponseContentLength];

	memcpy(_httpResponseContent, content, _httpResponseContentLength);
}

void chumby::HTTPResponse::addContent(std::string & content)
{
	_httpResponseContentLength = content.size();
	_httpResponseContent = new char[_httpResponseContentLength + 1];

	strcpy(_httpResponseContent, content.c_str());
}

char *chumby::HTTPResponse::getResponseString()
{
	std::string * response_header = getResponseHeader();
	_httpResponseLength = response_header->size();
	_httpResponseLength += _httpResponseContentLength;
	char * response = new char[_httpResponseLength + 1];
	memcpy(response, response_header->c_str(), response_header->size());
	if (_httpResponseContent) {
		memcpy(response + response_header->size(), _httpResponseContent, _httpResponseContentLength);
	}
	response[_httpResponseLength] = '\0';
	delete response_header;
	return response;
}

size_t chumby::HTTPResponse::getResponseLength()
{
	return _httpResponseLength;
}

std::string * chumby::HTTPResponse::getResponseHeader()
{
	std::string * response = new std::string();

	response->append("HTTP/1.1 ");

	switch(_httpResponseCode) {
		case HTTP_RESPONSE_CODE_OKAY:
			response->append("200 OK\r \n"); 
			break;
		case HTTP_RESPONSE_CODE_NOT_FOUND:
			response->append("404 Not Found\r\n");
			break;
		default:
			response->append("500 Internal Server Error\r\n");
			break;
	}
	if (_responseHeaders) {
		std::map<std::string, std::string>::const_iterator it;
		for (it=_responseHeaders->begin(); it!=_responseHeaders->end();++it) {
			response->append(it->first + ": " + it->second + "\r\n");
		}
	}

	if (_httpResponseContent) {
		std::ostringstream ss;
		ss << _httpResponseContentLength;
		response->append("Content-length: " + ss.str() + "\r\n");
	}
	response->append("Connection: close\r\n");
	response->append("\r\n");
	return response;
}
