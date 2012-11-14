
#ifndef HTTP_SERVER_H_
#define HTTP_SERVER_H_

#include <vector>
#include <unordered_map>

#include "server.h"
#include "http_exception.h"

namespace vv {
	using namespace std;

	class HTTPServer : public vv::Server {
	public:
		HTTPServer(unsigned short port);
		void handle();
	protected:
		static string accept(const Socket& client);	
		static string process(const string& message);
	private:
		static bool is_valid_http_message(string& message);
		static unordered_map<string, string> parse_post_data(const string& message);
		static unordered_map<string, string> parse_get_data(const string& message);
		static unordered_map<string, string> url_encoded_variables_to_map(const string& content);
		static unsigned char from_hex(unsigned char ch);
		static const string url_decode(const string& str);
	};
}

#endif
