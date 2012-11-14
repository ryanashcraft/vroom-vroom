
#ifndef HTTP_RESPONSE_H_
#define HTTP_RESPONSE_H_

#include <string>
#include <vector>

namespace vv {
	using namespace std;

	class HTTPResponse {
	public:
		HTTPResponse(int code, string& mime, string& content, vector<string>& headers, bool head=false);
		HTTPResponse(string& status, string& mime, string& content, vector<string>& headers, bool head=false);
		HTTPResponse(int code, bool head=false);
		string str();
		void set_head_only(bool head_only) {
			head_ = head_only;
		}
	private:
		string status_;
		string mime_;
		string content_;
		vector<string> headers_;
		bool head_;
	};
}

#endif
