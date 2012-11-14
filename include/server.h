
#ifndef SERVER_H_
#define SERVER_H_

#include "socket.h"

namespace vv {
	using namespace std;

	class Server {
	public:
		Server(unsigned short port);
		virtual ~Server();
		virtual void handle() = 0;
	protected:
		unsigned short port_;
		Socket socket_;
	};
}

#endif
