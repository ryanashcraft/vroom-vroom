
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <vector>
#include <unordered_map>

#include "server.h"
#include "http_exception.h"

class HTTPServer : public Server {
public:
	HTTPServer(unsigned short port);
	void handle();
protected:
	std::string accept(Socket& client);	
	virtual std::string process(const std::string& message);
	std::string OK(const std::string& message, std::vector<std::string>& headers, const std::string mime, bool no_body=false);
	std::string Redirect(const std::string& message, std::vector<std::string>& headers);
	std::string BadRequest(const std::string& message="");
	std::string NotFound(const std::string& message="");
	std::string InternalServerError(const std::string& message="");
	std::string NotImplemented(const std::string& message="");
private:
	bool is_valid_http_message(std::string& message);
	std::unordered_map<std::string, std::string> parse_post_data(const std::string& message);

	unsigned char from_hex(unsigned char ch) const;
	const std::string url_decode(const std::string& str) const;
};

#endif
