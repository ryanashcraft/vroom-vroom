
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>

#include "http_server.h"
#include "file_interpreter.h"
#include "path_resolution.h"
#include "regex_matcher.h"

using namespace std;
using namespace vv;

#define BUFFER_SIZE 128

HTTPServer::HTTPServer(unsigned short port) : Server(port) {

}

void HTTPServer::handle() {
	Socket client(socket_.accept());
	thread client_thread([this, client] () {
		bool should_continue = true;
		
		while (should_continue) {
			string request = accept(client);

			HTTPResponse reply = process(request);
			client.send(reply.str());
			should_continue = (reply.get_code() == 100);
		}
		client.close();
	});
	client_thread.detach();
}

string HTTPServer::accept(const Socket& client) {
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

	RegexMatcher content_length_regex("Content-Length:[ \t]*([0-9]+)");
	vector<string> matches = content_length_regex.find_matches(string(message)); 

	if (matches.size() >= 2) {
		content_length = atoi(matches[1].c_str());
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

HTTPResponse HTTPServer::process(const string& message) {
	string type;
	stringstream trimmer;
	trimmer << message;
	trimmer >> type;

	if (type == "HEAD" || type == "GET" || type == "POST") {
		string path;
		trimmer << message;
		trimmer >> path;

		path = vv::resolve_path(path, "/");

		path = path.substr(0, path.find("?"));

		if (path[path.length() - 1] == '/') {
			if (vv::file_exists(path + "index.ssjs")) {
				path += "index.ssjs";
			} else if (vv::file_exists(path + "index.html")) {
				path += "index.html";
			}
		}

		if (!vv::file_exists(path)) {
			return HTTPResponse(404, (type == "HEAD"));
		}

		RegexMatcher continue_regex("Expect:[\t ]*100-continue");
		vector<string> continue_matches = continue_regex.find_matches(string(message)); 

		if (continue_matches.size() > 0) {
			return HTTPResponse(100, (type == "HEAD"));
		}

		unique_ptr<FileInterpreter> interpreter = FileInterpreter::file_interpreter_for_path(path);

		interpreter.get()->set_get_data(parse_get_data(message));

		if (type == "POST") {
			interpreter.get()->set_post_data(parse_post_data(message));
		}

		string content = "";
		try {
			content = interpreter.get()->interpret();
		} catch (const HTTPException& e) {
			switch (e.code()) {
				case 404: return HTTPResponse(404, (type == "HEAD"));
				default:  return HTTPResponse(500, (type == "HEAD"));
			}
		}

		vector<string> headers = interpreter.get()->get_headers();
		string mime = interpreter.get()->mime();
		for (auto s = headers.begin(); s != headers.end(); ++s) {
			if (s->find("Location") == 0) {
				return HTTPResponse(302, mime, content, headers);
			}

			if (s->find("HTTP/") == 0) {
				string status(*s);
				headers.erase(s);
				return HTTPResponse(status, mime, content, headers, (type == "HEAD"));
			}
		}

		return HTTPResponse(300, mime, content, headers, (type == "HEAD"));
	}

	return HTTPResponse(502, (type == "HEAD"));
}

unordered_map<string, string> HTTPServer::parse_post_data(const string& message) {
	unordered_map<string, string> post_data;

	size_t message_start = message.find("\r\n\r\n") + 4;
	if (message_start != string::npos) {
		string content(message.substr(message_start));

		post_data = url_encoded_variables_to_map(content);
	}

	return post_data;
}

unordered_map<string, string> HTTPServer::parse_get_data(const string& message) {
	unordered_map<string, string> post_data;

	string first_line = message.substr(0, message.find("\r\n"));

	size_t params_start = first_line.find("?") + 1;
	if (params_start != string::npos) {
		size_t params_end = first_line.find_first_of(' ', params_start + 1);

		string content(first_line.substr(params_start, params_end - params_start));

		post_data = url_encoded_variables_to_map(content);
	}

	return post_data;
}

unordered_map<string, string> HTTPServer::url_encoded_variables_to_map(const string& content) {
	unordered_map<string, string> map;

	stringstream ss(content);
	do {
		string lvalue, rvalue;
		
		if (!getline(ss, lvalue, '=')) {
			break;
		}

		if (!getline(ss, rvalue, '&')) {
			break;
		}

	 	map[url_decode(lvalue)] = url_decode(rvalue);
	} while (ss);

	return map;
}

/**
 * @see http://dlib.net/dlib/server/server_1.h.html
 */
unsigned char HTTPServer::from_hex(unsigned char ch) {
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

const string HTTPServer::url_decode(const string& str) {
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
