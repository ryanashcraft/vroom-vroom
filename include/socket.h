
#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include <string>

class Socket {
public:
	Socket(std::string address, int port);
	Socket(int descriptor_);
	~Socket();
	void bind(unsigned short port);
	void listen(unsigned int max_requests);
	Socket accept();
	int receive(unsigned int buffer_size, char* buffer);
	void send(char* buffer, int message_size);
private:
	int descriptor_;
};

#endif