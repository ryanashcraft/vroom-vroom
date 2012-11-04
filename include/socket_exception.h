
#ifndef SOCKET_EXCEPTION_H
#define SOCKET_EXCEPTION_H

class SocketException : public std::exception {
public:
	explicit SocketException(const std::string& message) : message_("SocketException: " + message) {

	}

	const char* what() const throw() {
		return message_.c_str();
	}
private:
	std::string message_;
};

#endif
