
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex.h>
#include <cstdlib>

#include "http_server.h"
#include "date.h"
#include "file_interpreter.h"
#include "path_resolution.h"

using namespace std;

#define BUFFER_SIZE 128

HTTPServer::HTTPServer(unsigned short port) : Server(port) {

}

void HTTPServer::handle() {
	Socket client = socket_.accept();
	string reply = process(accept(client));
	client.send(reply);
	client.close();
}

string HTTPServer::accept(Socket& client) {
	char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
	int received_size = 0;

	string message = "";
	received_size = client.receive(BUFFER_SIZE, buffer);
    message.append(buffer, received_size);

    while (!is_valid_http_message(message) && received_size > 0) {
	    memset(buffer, 0, BUFFER_SIZE);
		received_size = client.receive(BUFFER_SIZE, buffer);
	    message.append(buffer, received_size);
    }

    return message;
}

bool HTTPServer::is_valid_http_message(string& message) {
	int content_length = 0;

	regmatch_t matches[2];
	regex_t content_length_regex;
	::regcomp(&content_length_regex, "Content-Length:[ \t]*([0-9]+)", REG_EXTENDED);
	int regexec_retval = ::regexec(&content_length_regex, message.c_str(), 2, matches, 0);
	::regfree(&content_length_regex);

	if (regexec_retval == 0 && matches[1].rm_so >= 0 && matches[1].rm_eo < message.length()) {
		content_length = ::atoi(message.substr(matches[1].rm_so, matches[1].rm_eo - matches[1].rm_so).c_str());
	}

	size_t message_begin = message.find("\r\n\r\n");

	if (message_begin == string::npos) {
		return false;
	}

	if (message.substr(message_begin).length() - 4 == content_length) {
		return true;
	}

	return false;
}

string HTTPServer::process(const string& message) {
	string type;
	stringstream trimmer;
	trimmer << message;
	trimmer >> type;

	if (type == "HEAD" || type == "GET" || type == "POST") {
		string path;
		trimmer << message;
		trimmer >> path;

		path = vv::resolve_path(path, "/");

		if (path[path.length() - 1] == '/') {
			if (vv::file_exists(path + "index.ssjs")) {
				path += "index.ssjs";
			} else if (vv::file_exists(path + "index.html")) {
				path += "index.html";
			}
		}

		if (!vv::file_exists(path)) {
			return NotFound();
		}

		unique_ptr<FileInterpreter> interpreter = FileInterpreter::file_interpreter_for_path(path);

		if (type == "POST") {
			interpreter.get()->set_post_data(parse_post_data(message));
		}

		string content = "";
		try {
			content = interpreter.get()->interpret();
		} catch (const HTTPException& e) {
			switch (e.code()) {
				case 404: return NotFound();
				default:  return InternalServerError();
			}
		}

		return OK(content, interpreter.get()->mime(), (type == "HEAD"));
	}

	return NotImplemented();
}

vector<string> HTTPServer::parse_post_data(const string& message) {
	vector<string> post_data;

	size_t message_start = message.find("\r\n\r\n");
	if (message_start != string::npos) {
	 	post_data.push_back(message.substr(message_start + 4));
	}

	return post_data;
}

string HTTPServer::OK(const string& message, const string mime, bool no_body) {
	stringstream response;
	response << "HTTP/1.1 200 OK" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Accept-Ranges: bytes" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << "Connection: close" << endl;
	response << "Content-Type: " << mime << endl << endl;
	if (!no_body) {
		response << message << endl;
	}
	return response.str();
}

string HTTPServer::BadRequest(const string& message) {
	stringstream response;
	response << "HTTP/1.0 400 Bad Request" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << message;
	return response.str();
}

string HTTPServer::NotFound(const string& message) {
	stringstream response;
	response << "HTTP/1.0 404 Not Found" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << message;
	return response.str();
}

string HTTPServer::InternalServerError(const string& message) {
	stringstream response;
	response << "HTTP/1.0 500 Internal Server Error" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << message;
	return response.str();
}

string HTTPServer::NotImplemented(const string& message) {
	stringstream response;
	response << "HTTP/1.0 502 Not Implemented" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << message;
	return response.str();
}
