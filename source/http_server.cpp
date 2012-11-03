
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "http_server.h"

using namespace std;

#define BUFFER_SIZE 32

HTTPServer::HTTPServer(unsigned short port) : Server(port) {
	cout << "HTTPServer started" << endl;
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
    message.append(buffer);

    while (!is_valid_http_message(message) && received_size > 0) {
	    memset(buffer, 0, BUFFER_SIZE);
		received_size = client.receive(BUFFER_SIZE, buffer);
	    message.append(buffer);
    }

    return message;
}


string HTTPServer::process(const string& message) {
	string path;
	std::stringstream trimmer;
	trimmer << message;
	trimmer >> path;
	return file_to_string(path);
}

string HTTPServer::file_to_string(const string& path) {
	ifstream file(path);

	if (!file.is_open()) {
		fprintf(stderr, "ERROR: (404) file not found at path %s\n", path.c_str());
		// exception
		return "";
	}

	std::stringstream buffer;
    buffer << file.rdbuf();
	file.close();

	return buffer.str();
}

bool HTTPServer::is_valid_http_message(string& message) {
	if (message[message.length() - 1] == '\n') {
		return true;
	}

	return false;
}
