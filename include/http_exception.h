
#ifndef HTTP_EXCEPTION_H_
#define HTTP_EXCEPTION_H_

#include <string>

namespace vv {
	using namespace std;

	/**
	 * Subclass of exception to throw when an unexpected event occurs that disrupts normal
	 * execution for generating an HTTP response.
	 */
	class HTTPException : public exception {
	public:
		/**
		 * Creates an HTTP exception with an HTTP code and related file path.
		 * @param code	an HTTP code
		 * @param path	the file path from the HTTP request
		 */
		explicit HTTPException(unsigned int code, const string& path) : code_(code), path_(path) {

		}

		/**
		 * Returns a string to describe the exception.
		 * @return a character string to describe the exception
		 */
		const char* what() const throw() {
			return "HTTPException";
		}

		/**
		 * Returns the HTTP code related to the exception.
		 * @return the HTTP code
		 */
		const int code() const throw() {
			return code_;
		}
	private:
		/**
		 * The HTTP code related to the exception.
		 */
		unsigned int code_;

		/**
		 * The requested file path related to the exception.
		 */
		string path_;
	};
}

#endif
