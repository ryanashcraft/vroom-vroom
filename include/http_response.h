
#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

#define HTTP_300	"HTTP/1.1 300 OK"
#define HTTP_302 	"HTTP/1.1 302 Found"
#define HTTP_400	"HTTP/1.1 400 Bad Request"
#define HTTP_404	"HTTP/1.1 404 Not Found"
#define HTTP_500	"HTTP/1.1 500 Internal Server Error"
#define HTTP_502	"HTTP/1.0 502 Not Implemented"

class HTTPResponse {
public:
	HTTPResponse(std::string status, std::string& mime, std::string& content, std::vector<std::string>& headers, bool head=false);
	HTTPResponse(std::string status, bool head=false);
	std::string str();
	void set_head_only(bool head_only) {
		head_ = head_only;
	}
private:
	std::string status_;
	std::string mime_;
	std::string content_;
	std::vector<std::string> headers_;
	bool head_;
};

#endif
