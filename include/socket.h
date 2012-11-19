
#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <string>

#include "socket_exception.h"

namespace vv {
	using namespace std;

	class Socket {
	public:
		Socket();
		// Socket(Socket &s);
		Socket(int descriptor_);
		void bind(unsigned short port);
		void listen(unsigned int max_requests=5);
		Socket accept() const;
		int receive(unsigned int buffer_size, char* buffer) const;
		void send(string message) const;
		void close() const;
	private:
		int descriptor_;
	};
}

#endif