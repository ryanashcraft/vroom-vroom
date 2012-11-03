
#ifndef SERVER_H_
#define SERVER_H_

#include "socket.h"

class Server {
public:
	Server(unsigned short port);
	void handle();
private:
	unsigned short port_;
	Socket socket_;
};

#endif
