
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "http_server.h"
#include "date.h"
#include "file_interpreter.h"

using namespace std;

#define BUFFER_SIZE 32

HTTPServer::HTTPServer(unsigned short port) : Server(port) {

}

void HTTPServer::handle() {
	Socket client = socket_.accept();
	string reply = process(accept(client));

	cout << "========================" << endl;
	cout << reply << endl;

	client.send(reply);
	client.close();
}

string HTTPServer::accept(Socket& client) {
	char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
	int received_size = 0;

	string message = "";
	received_size = client.receive(BUFFER_SIZE, buffer);
    message.append(buffer);

    while (!is_valid_http_message(message) && received_size > 0) {
	    memset(buffer, 0, BUFFER_SIZE);
		received_size = client.receive(BUFFER_SIZE, buffer);
	    message.append(buffer);
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

		// remove absolute URL forward slash
		path.erase(0, 1);

		unique_ptr<FileInterpreter> interpreter = FileInterpreter::file_interpreter_for_path(path);

		string content = "";
		try {
			content = interpreter.get()->interpret();
		} catch (const HTTPException& e) {
			fprintf(stderr, "NOT FOUND: %s\n", path.c_str());
			return NotFound();
		}

		return OK(content);
	}

	fprintf(stderr, "BAD REQUEST: %s\n", message.c_str());
	return BadRequest("");
}

bool HTTPServer::is_valid_http_message(string& message) {
	if (message[message.length() - 1] == '\n') {
		return true;
	}

	return false;
}

string HTTPServer::OK(const string& message) {
	stringstream response;
	response << "HTTP/1.0 200 OK" << endl;
	response << "Date: " << Date::now("%a, %d %b %Y %H:%M:%S %Z") << endl;
	response << "Content-Type: text/html" << endl;
	response << "Content-Length: " << message.length() - 1 << endl;
	response << message;
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
