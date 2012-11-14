
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <vector>
#include <unordered_map>

#include "server.h"
#include "http_exception.h"

namespace vv {
	class HTTPServer : public vv::Server {
	public:
		HTTPServer(unsigned short port);
		void handle();
	protected:
		static std::string accept(const Socket& client);	
		static std::string process(const std::string& message);
	private:
		static bool is_valid_http_message(std::string& message);
		static std::unordered_map<std::string, std::string> parse_post_data(const std::string& message);
		static std::unordered_map<std::string, std::string> parse_get_data(const std::string& message);
		static std::unordered_map<std::string, std::string> url_encoded_variables_to_map(const std::string& content);
		static unsigned char from_hex(unsigned char ch);
		static const std::string url_decode(const std::string& str);
	};
}

#endif
