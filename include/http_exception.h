
#ifndef HTTP_EXCEPTION_H_
#define HTTP_EXCEPTION_H_

#include <string>

namespace vv {
	using namespace std;

	class HTTPException : public exception {
	public:
		explicit HTTPException(unsigned int code, const string& path) : code_(code), path_(path) {

		}
		const char* what() const throw() {
			return "HTTPException";
		}
		const int code() const throw() {
			return code_;
		}
	private:
		unsigned int code_;
		string path_;
	};
}

#endif
