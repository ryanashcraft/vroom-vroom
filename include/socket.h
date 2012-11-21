
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

	/**
	 * Represents a TCP network socket.
	 */
	class Socket {
	public:
		/**
		 * Creates a Socket instance.
		 */
		Socket();

		/**
		 * Creates a Socket instance with a given socket descriptor.
		 */
		Socket(int descriptor_);

		/**
		 * Binds the socket to a given port.
		 * @param port the port to bind to
		 */
		void bind(unsigned short port);

		/**
		 * Listens to requests from incoming connections.
		 * @param max_requests the maximum number of simultaneous requests
		 */
		void listen(unsigned int max_requests = 5);

		/**
		 * Accepts an incoming connection from a client.
		 * @return the Socket instance representing the client's connection
		 */
		Socket accept() const;

		/**
		 * Receives data from the socket.
		 * @param  buffer_size the size of the character buffer
		 * @param  buffer      the character buffer for the message
		 * @return             the size of the received message
		 */
		int receive(unsigned int buffer_size, char* buffer) const;

		/**
		 * Sends a string message to the socket.
		 * @param message the message to send
		 */
		void send(string message) const;

		/**
		 * Closes the socket.
		 */
		void close() const;
	private:
		/**
		 * The socket descriptor.
		 */
		int descriptor_;
	};
}

#endif