
#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

namespace vv {
	using namespace std;

	class SocketException : public exception {
	public:
		explicit SocketException(const string& message) : message_("SocketException: " + message) {

		}

		const char* what() const throw() {
			return message_.c_str();
		}
	private:
		string message_;
	};
}

#endif
