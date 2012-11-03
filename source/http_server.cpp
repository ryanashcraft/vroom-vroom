
#include <string>

#include "http_server.h"

using namespace std;

#define BUFFER_SIZE 32

HTTPServer::HTTPServer(unsigned short port) : Server(port) {

}

void HTTPServer::handle() {
	Socket client = socket_.accept();
	string reply = process(accept(client));
	client.send(reply);
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


string HTTPServer::process(const string message) {
	return "";
}

bool HTTPServer::is_valid_http_message(string& message) {
	if (message[message.length() - 1] == '\n') {
		return true;
	}

	return false;
}
