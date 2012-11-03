
#include <sstream>

#include "socket.h"

using namespace std;

Socket::Socket() {
	if ((descriptor_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		throw SocketException("socket could now be initialized");
    }
}

Socket::Socket(int descriptor) : descriptor_(descriptor) {

}

void Socket::bind(unsigned short port) {
	struct sockaddr_in address;

    memset(&address, 0, sizeof(address));   /* Zero out structure */
    address.sin_family = AF_INET;                /* Internet address family */
    address.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
    address.sin_port = htons(port);      /* Local port */

    /* Bind to the local address */
    if (::bind(descriptor_, (struct sockaddr *) &address, sizeof(address)) < 0) {
        stringstream message;
        message << "socket could now be binded to port " << port;
    	throw SocketException(message.str());
    }
}

void Socket::listen(unsigned int max_requests) {
	/* Mark the socket so it will listen for incoming connections */
    if (::listen(descriptor_, max_requests) < 0) {
    	// exception
    }
}

Socket Socket::accept() {
	int client_descriptor;
	struct sockaddr_in client_address; /* Client address */
	unsigned int client_address_size = sizeof(client_address);

	/* Wait for a client to connect */
    if ((client_descriptor = ::accept(descriptor_, (struct sockaddr *) &client_address, &client_address_size)) < 0) {
    	// exception
   }

   return Socket(client_descriptor);
}

int Socket::receive(unsigned int buffer_size, char* buffer) {
    int message_size;                    /* Size of received message */

    /* Receive message from client */
    if ((message_size = ::recv(descriptor_, buffer, buffer_size, 0)) < 0) {
    	throw SocketException("socket receive failed");	
    }

    return message_size;
}

void Socket::send(string& message) {
	if (::send(descriptor_, message.c_str(), message.length(), 0) != message.length()) {
		// exception
	}
}

void Socket::close() {
    ::close(descriptor_);
}
