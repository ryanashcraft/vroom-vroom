
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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


string HTTPServer::process(const string& message) {
	string type;
	std::stringstream trimmer;
	trimmer << message;
	trimmer >> type;

	if (type == "GET") {
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

		unique_ptr<FileInterpreter> interpreter = FileInterpreter::file_interpreter_for_path(path);

		string content = "";
		try {
			content = interpreter.get()->interpret();
		} catch (const HTTPException& e) {
			switch (e.code()) {
				case 404: return NotFound();
				default:  return InternalServerError();
			}
		}

		return OK(content, interpreter.get()->mime());
	}

	return NotImplemented();
}

bool HTTPServer::is_valid_http_message(string& message) {
	if (message[message.length() - 1] == '\n') {
		return true;
	}

	return false;
}

string HTTPServer::OK(const string& message, const string mime) {
	stringstream response;
	response << "HTTP/1.1 200 OK" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Accept-Ranges: bytes" << endl;
	response << "Content-Length: " << message.length() << endl;
	response << "Connection: close" << endl;
	response << "Content-Type: " << mime << endl << endl;
	response << message << endl;
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
