
#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

namespace vv {
	class HTTPResponse {
	public:
		HTTPResponse(int code, std::string& mime, std::string& content, std::vector<std::string>& headers, bool head=false);
		HTTPResponse(std::string& status, std::string& mime, std::string& content, std::vector<std::string>& headers, bool head=false);
		HTTPResponse(int code, bool head=false);
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
}

#endif
