
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex.h>
#include <cstdlib>

#include "http_server.h"
#include "file_interpreter.h"
#include "path_resolution.h"
#include "http_response.h"

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

	size_t message_start = message.find("\r\n\r\n");

	if (message_start == string::npos) {
		return false;
	}

	if (message.substr(message_start).length() - 4 == content_length) {
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
			HTTPResponse response(HTTP_404, (type == "HEAD"));
			return response.str();
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
				case 404: return HTTPResponse(HTTP_404, (type == "HEAD")).str();
				default:  return HTTPResponse(HTTP_500, (type == "HEAD")).str();
			}
		}

		vector<string> headers = interpreter.get()->get_headers();
		string mime = interpreter.get()->mime();
		for (auto s = headers.begin(); s != headers.end(); ++s) {
			if (s->find("Location") == 0) {
				return HTTPResponse(HTTP_302, mime, content, headers).str();
			}

			if (s->find("HTTP/") == 0) {
				string status(*s);
				headers.erase(s);
				return HTTPResponse(status, mime, content, headers, (type == "HEAD")).str();
			}
		}

		return HTTPResponse(HTTP_300, mime, content, headers, (type == "HEAD")).str();
	}

	return HTTPResponse(HTTP_502, (type == "HEAD")).str();
}

unordered_map<string, string> HTTPServer::parse_post_data(const string& message) {
	unordered_map<string, string> post_data;

	size_t message_start = message.find("\r\n\r\n");
	if (message_start != string::npos) {
		string content(message.substr(message_start + 4));

		stringstream ss(content);
		do {
			string lvalue, rvalue;
			
			if (!getline(ss, lvalue, '=')) {
				break;
			}

			if (!getline(ss, rvalue, '&')) {
				break;
			}

		 	post_data[lvalue] = rvalue;
		} while (ss);
	}

	return post_data;
}

// http://dlib.net/dlib/server/server_http_1.h.html
unsigned char HTTPServer::from_hex(unsigned char ch) const {
	if (ch <= '9' && ch >= '0')
	    ch -= '0';
	else if (ch <= 'f' && ch >= 'a')
	    ch -= 'a' - 10;
	else if (ch <= 'F' && ch >= 'A')
	    ch -= 'A' - 10;
	else 
	    ch = 0;

	return ch;
}

const string HTTPServer::url_decode(const string& str) const {
	string result;
	string::size_type i;
	for (i = 0; i < str.size(); ++i) {
		if (str[i] == '+') {
			result += ' ';
		} else if (str[i] == '%' && str.size() > i+2) {
			const unsigned char ch1 = from_hex(str[i+1]);
			const unsigned char ch2 = from_hex(str[i+2]);
			const unsigned char ch = (ch1 << 4) | ch2;
			result += ch;
			i += 2;
	    } else {
			result += str[i];
		}
	}

	return result;
}
