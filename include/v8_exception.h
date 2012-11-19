
#ifndef V8_EXCEPTION_H_
#define V8_EXCEPTION_H_

#include <v8.h>
#include <string>

namespace vv {
	using namespace std;

	/**
	 * Subclass of exception to throw when an unexpected error occurs using the V8 library.
	 */
	class V8Exception : public exception {
	public:
		/**
		 * Creates a V8Exception with a V8 try_catch instance and a file path.
		 * @see http://v8.googlecode.com/svn/trunk/samples/shell.cc
		 * @param  try_catch pointer to the v8 TryCatch instance
		 * @param  path      the path of the file interpreted by V8
		 */
		explicit V8Exception(v8::TryCatch* try_catch, const string& path);

		/**
		 * Returns the error message string.
		 */
		const char* what() const throw() {
			return message_.c_str();
		}
	private:
		/**
		 * The error message.
		 */
		string message_;
	};
}

#endif
