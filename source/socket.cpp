
#include <sstream>

#include "socket.h"

using namespace std;
using namespace vv;

Socket::Socket() {
	if ((descriptor_ = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		throw SocketException("socket could now be initialized");
    }
}

Socket::Socket(int descriptor) : descriptor_(descriptor) {

}

void Socket::bind(unsigned short port) {
	struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    if (::bind(descriptor_, (struct sockaddr *) &address, sizeof(address)) < 0) {
        stringstream message;
        message << "socket could now be binded to port " << port;
    	throw SocketException(message.str());
    }
}

void Socket::listen(unsigned int max_requests) {
    if (::listen(descriptor_, max_requests) < 0) {
    	// @todo throw SocketException
    }
}

Socket Socket::accept() const {
	int client_descriptor;
	struct sockaddr_in client_address; /* Client address */
	unsigned int client_address_size = sizeof(client_address);

	/* Wait for a client to connect */
    if ((client_descriptor = ::accept(descriptor_, (struct sockaddr *) &client_address, &client_address_size)) < 0) {
    	// @todo throw SocketException
   }

   return Socket(client_descriptor);
}

int Socket::receive(unsigned int buffer_size, char* buffer) const {
    int message_size;

    if ((message_size = ::recv(descriptor_, buffer, buffer_size, 0)) < 0) {
    	throw SocketException("socket receive failed");	
    }

    return message_size;
}

void Socket::send(string message) const {
	if ((int)::send(descriptor_, message.c_str(), message.length(), 0) != (int)message.length()) {
		// @todo throw SocketException
	}
}

void Socket::close() const {
    ::close(descriptor_);
}
