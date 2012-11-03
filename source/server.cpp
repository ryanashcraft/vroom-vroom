
#include <iostream>

#include "server.h"

#define BUFFER_SIZE 32

using namespace std;

Server::Server(unsigned short port) : port_(port) {
	socket_.bind(port_);
	socket_.listen();
}

void Server::handle() {
	Socket client = socket_.accept();
	char buffer[BUFFER_SIZE];
	int message_size = 0;

    do {
	    message_size = client.receive(BUFFER_SIZE, buffer);
        client.send(buffer, message_size);
    } while (message_size > 0);
}
