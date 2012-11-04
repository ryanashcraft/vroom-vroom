
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include "server.h"

class HTTPServer : public Server {
public:
	HTTPServer(unsigned short port);
	void handle();
protected:
	std::string accept(Socket& client);	
	virtual std::string process(const std::string& message);
	std::string OK(const std::string& message);
	std::string BadRequest(const std::string& message);
private:
	bool is_valid_http_message(std::string& message);
	std::string file_to_string(const std::string& path);
};

#endif
