
#ifndef V8_EXCEPTION_H_
#define V8_EXCEPTION_H_

#include <v8.h>
#include <string>

namespace vv {
	using namespace std;

	/**
	 * Exception to throw when an error occurs using the V8 library.
	 */
	class V8Exception : public exception {
	public:
		// from http://v8.googlecode.com/svn/trunk/samples/shell.cc
		explicit V8Exception(v8::TryCatch* try_catch, const string& path);
		const char* what() const throw() {
			return message_.c_str();
		}
	private:
		string message_;
	};
}

#endif
