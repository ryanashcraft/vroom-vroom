
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "server.h"
#include "http_exception.h"

class HTTPServer : public Server {
public:
	HTTPServer(unsigned short port);
	void handle();
protected:
	std::string accept(Socket& client);	
	virtual std::string process(const std::string& message);
	std::string OK(const std::string& message, const std::string mime);
	std::string BadRequest(const std::string& message);
	std::string NotFound(const std::string& message="");
private:
	bool is_valid_http_message(std::string& message);
};

#endif
