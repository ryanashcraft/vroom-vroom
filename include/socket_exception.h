
#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

namespace vv {
	using namespace std;

	/**
	 * Subclass of exception to throw when an unexpected event occurs when dealing with Sockets.
	 */
	class SocketException : public exception {
	public:
		/**
		 * Creates a socket exception with an error message.
		 * @param  message the error message
		 * @return         the socket exception
		 */
		explicit SocketException(const string& message) : message_("SocketException: " + message) {

		}

		/**
		 * Returns the error message string.
		 * @param the error message string
		 */
		const char* what() const throw() {
			return message_.c_str();
		}
	private:
		/**
		 * The error message.
		 */
		string message_;
	};
}

#endif
