
#include <iostream>

#include "server.h"

using namespace std;

Server::Server(unsigned short port) : port_(port) {
	try {
		socket_.bind(port_);
	} catch(SocketException& e) {
		cerr << e.what() << endl;

		exit(-1);
	}

	cout << "Server started on port " << port << endl;

	socket_.listen();
}
