
#ifndef SERVER_H_
#define SERVER_H_

#include "socket.h"

namespace vv {
	using namespace std;

	/**
	 * Handles opening and closing of a port for handling incoming connections.
	 */
	class Server {
	public:
		/**
		 * Creates a server instance on a specific port.
		 * @param port	the port for the server socket
		 */
		Server(unsigned short port);

		/**
		 * Closes the server port.
		 */
		virtual ~Server();

		/**
		 * Abstract function for handling client requests.
		 */
		virtual void handle() = 0;
	protected:
		/**
		 * The port of the server socket.
		 */
		unsigned short port_;

		/**
		 * The Socket instance used by the server.
		 */
		Socket socket_;
	};
}

#endif
