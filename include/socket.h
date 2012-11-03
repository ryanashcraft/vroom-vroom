
#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <string>

#include "socket_exception.h"

class Socket {
public:
	Socket();
	Socket(int descriptor_);
	void bind(unsigned short port);
	void listen(unsigned int max_requests=5);
	Socket accept();
	int receive(unsigned int buffer_size, char* buffer);
	void send(std::string& message);
	void close();
private:
	int descriptor_;
};

#endif