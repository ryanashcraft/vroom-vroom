
#ifndef V8_EXCEPTION_
#define V8_EXCEPTION_

#include <v8.h>
#include <string>

namespace vv {
	class V8Exception : public std::exception {
	public:
		// from http://v8.googlecode.com/svn/trunk/samples/shell.cc
		explicit V8Exception(v8::TryCatch* try_catch, const std::string& path);
		const char* what() const throw() {
			return message_.c_str();
		}
	private:
		std::string message_;
	};
}

#endif
