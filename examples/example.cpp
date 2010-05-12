/*
 * example
 * Copyright (C) Chumby Industries, 2007
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
#include <chumby_httpd/chumby_http_content_handler.h>

int main(int argc, char **argv) {
	chumby::HTTPServer server(8080, 1);
	server.addContentHandler(new chumby::HTTPSimpleContentHandler(
		"/blah",
		"<html><head></head><body><b> blah </b></body></html>",
		"text/html"));

	server.addContentHandler(new chumby::HTTPSimpleFileContentHandler(
		"/passwd",
		"/etc/passwd",
		"text/plain"));

	server.addContentHandler(new chumby::HTTPEchoContentHandler());

	server.start();
}
