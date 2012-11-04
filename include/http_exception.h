
#ifndef HTTP_EXCEPTION_H_
#define HTTP_EXCEPTION_H_

#include <string>

class HTTPException : public std::exception {
public:
	explicit HTTPException(unsigned int code, const std::string& path) : code_(code), path_(path) {

	}
	const char* what() const throw() {
		return "HTTPException";
	}
private:
	unsigned int code_;
	std::string path_;
};

#endif
